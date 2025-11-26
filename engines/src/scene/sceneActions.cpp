#include <nlohmann/json.hpp>
#include "brep/body.h"
#include "utils/vec3d.h"
#include "brep/navigate.h"
#include "brep/tessellate.h"
#include "frep/fevaluate.h"
#include "document/document.h"

using json = nlohmann::json;

/**
 * Scene Actions are dispatched by action routines to keep the graphics scene in sync with the models.
 * The graphics scene is in fact managed in the client application. 
 * Hence these actions are so-called "client actions".
 * This module provides utilities to construct the payloads for those actions.
 */

namespace e2 {

    void dispatchClientActionsForAcorn(Document& doc, const Body& acornBody) {
        // The payload is a point
        const Vec3d& position = acornBody.cell(0).support().position();
        json payload = json::object({{"position", json::array({position.x(), position.y(), position.z()})}});
        doc.dispatchClientAction({"Gfx::addPoint", payload});
    }

    void dispatchClientActionsForSketch(Document& doc, const Body& sketchBody) {
        // The payload for each edge is a polyline
        auto edges = getKSkeleton(1, sketchBody);
        for (const auto& edgeIndex : edges) {
            auto tessellatedPointsPtr = tessellateEdge(edgeIndex, sketchBody);
            std::vector<json> positions;
            for (const auto& point : *tessellatedPointsPtr) {
                positions.push_back(json::array({point.x(), point.y(), point.z()}));
            }
            json payload = json::object({{"positions", positions}});
            doc.dispatchClientAction({"Gfx::addPolyline", payload});
            delete tessellatedPointsPtr;
        }
    }

    void dispatchClientActionsForProfile(Document& doc, const Body& profileBody) {
        // The payload is a collection of "paths", representing the outline of the given body.
        CellIndex faceIndex = getKSkeleton(2, profileBody)[0];
        auto edges = getKBoundary(1, faceIndex, profileBody);
        std::vector<json> paths;
        for (const auto& edgePair : edges) {
            CellIndex edgeIndex = edgePair.first;
            auto tessellatedPointsPtr = tessellateEdge(edgeIndex, profileBody);
            std::vector<json> path;
            for (const auto& point : *tessellatedPointsPtr) {
                    path.push_back(json::array({point.x(), point.y()}));
            }
            if (edgePair.second == -1) {
                std::reverse(path.begin(), path.end());
            }
            paths.push_back(path);
            delete tessellatedPointsPtr;
        }

        json payload = json::object({{"paths", paths}});
        doc.dispatchClientAction({"Gfx::addProfile", payload});
    }

    static std::array<int, 4> lerp(const std::array<int,4>& colorA, const std::array<int,4>& colorB, double t) {
        int r = static_cast<int>((1 - t) * colorA[0] + t * colorB[0]);
        int g = static_cast<int>((1 - t) * colorA[1] + t * colorB[1]);
        int b = static_cast<int>((1 - t) * colorA[2] + t * colorB[2]);
        int a = static_cast<int>((1 - t) * colorA[3] + t * colorB[3]);
        return {r, g, b, a};
    }

    static std::array<int, 4> distanceToRgb(double d, double range=5.0) {
    
        // Map a signed distance value to an RGB integer.

        // Distances <= -range map to opaque black
        // Distances >= range map to transparent white
        // Distance between -range and 0 map to a gradient from opaque black to light blue at the zero level surface
        // Distances between 0 and +range map to a gradient from pink at the zero level surface to transparent white

        const auto black = std::array<int,4>{0,0,0,255};
        const auto white = std::array<int,4>{255,255,255,0};
        const auto lightBlue = std::array<int,4>{173,216,230,255};
        const auto pink = std::array<int,4>{255,105,180,255};

        if (d <= -range) {
            return black;
        } 
        else if (d >= range) {
            return white;
        } 
        else if (d < 0) {
            double t = (d + range) / range;
            return lerp(black, lightBlue, t);
        } 
        else {
            double t = d / range;
            return lerp(pink, white, t);
        }
    }

    void dispatchClientActionsForObject(Document& doc, const FObject& fobject) {

        // Generate a stack of images representing the SDF of the given object
        int imageWidth = 100;
        int imageHeight = 100;
        int numSlabsX = 1;       // only one slab in X and Y for now, until "z" is replaced by "position" in the addPlane action (to allow placing the slabs arbitrarily in XY)
        int numSlabsY = 1;
        int numSlices = 5;

        double width = 20.0;
        double height = 20.0;
        double depth = 20.0;
        double pixelWidth = width / (imageWidth * numSlabsX);     // size of each pixel in world units
        double pixelHeight = height / (imageHeight * numSlabsY); // size of each pixel in world units
        double sliceThickness = depth/(numSlices - 1);   // distance between slices

        std::vector<std::vector<int>> imageStack; // stack of images, each image is a vector of RGBA integers    
        for (int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex) {
            double z = -((numSlices / 2) * sliceThickness) + sliceIndex * sliceThickness;
            for (int ySlabIndex = 0; ySlabIndex < numSlabsY; ++ySlabIndex) {
                for (int xSlabIndex = 0; xSlabIndex < numSlabsX; ++xSlabIndex) {    
                    std::vector<int> image; // single image as a vector of RGB integers
                    for (int yIndex = ySlabIndex * imageHeight; yIndex < (ySlabIndex + 1) * imageHeight; ++yIndex) {
                        double y = -((numSlabsY * imageHeight / 2) * pixelHeight) + yIndex * pixelHeight;
                        for (int xIndex = xSlabIndex * imageWidth; xIndex < (xSlabIndex + 1) * imageWidth; ++xIndex) {
                            double x = -((numSlabsX * imageWidth / 2) * pixelWidth) + xIndex * pixelWidth;
                            Vec3d position(x, y, z);
                            double sdfValue;
                            bool evalResult = e2::evaluate(fobject, position, sdfValue);
                            if (!evalResult) {
                                sdfValue = std::numeric_limits<double>::max(); // assign a large value if evaluation fails
                            }
                            auto rgb = distanceToRgb(sdfValue, 5.0); // map sdfValue to RGB
                            image.push_back(rgb[0]); // R
                            image.push_back(rgb[1]); // G
                            image.push_back(rgb[2]); // B
                            image.push_back(rgb[3]); // A
                        }
                    }
                    imageStack.push_back(image);
                }
            }
        }

        // Construct and dispatch the client actions to display the images
        for(int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex) {
            double z = -((numSlices / 2) * sliceThickness) + sliceIndex * sliceThickness;
            for (int slabIndex = 0; slabIndex < numSlabsX * numSlabsY; ++slabIndex) {   
                auto& image = imageStack[sliceIndex + slabIndex];
                json payload = json::object({
                    {"width", width},
                    {"height", height},
                    {"z", z},
                    {"texture", 
                        json::object({
                            {"width", imageWidth},
                            {"height", imageHeight},
                            {"data", image}
                        })
                    }
                });
            doc.dispatchClientAction({"Gfx::addPlane", payload});
            }
        }
    }
};


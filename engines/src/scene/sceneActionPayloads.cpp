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
    // This function returns the JSON payload for the addGProfile action.
    void getPayloadForAddGProfile(const Body& profileBody, json& payload) {
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

        payload = json::object({{"paths", paths}});
    }

    static std::array<int, 4> distanceToRgb(double d, double range=10.0){
        
        // Map a signed distance value to an RGB integer.
        // Distances <= -range map to blue (0x0000FF)
        // Distances >= range map to red (0xFF0000)
        // Distances in between map to a gradient from blue to red via white.

        if (d <= -range) {
            return {0, 0, 255, 255}; // blue
        } 
        else if (d >= range) {
            return {255, 0, 0, 255}; // red
        } 
        else if (d < 0) {
            // interpolate from blue to white
            double t = (d + range) / range; // t goes from 0 to 1 as d goes from -range to 0
            int r = static_cast<int>(t * 255);
            int g = static_cast<int>(t * 255);
            int b = 255;
            return {r, g, b, 255};
        } 
        else {
            // interpolate from white to red
            double t = d / range; // t goes from 0 to 1 as d goes from 0 to range
            int r = 255;
            int g = static_cast<int>((1 - t) * 255);
            int b = static_cast<int>((1 - t) * 255);
            return {r, g, b, 255};
        }
    }

    void getClientActionsForAddFObject(const FObject& fobject, std::vector<ActionSpec>& actions) {

        // Generate a stack of images representing the SDF of the given FObject.
        
        int imageWidth = 32;
        int imageHeight = 32;
        int numSlices = 3;

        double width = 20.0;
        double height = 20.0;
        double depth = 20.0;
        double pixelWidth = width / imageWidth;     // size of each pixel in world units
        double pixelHeight = height / imageHeight; // size of each pixel in world units
        double sliceThickness = depth/(numSlices - 1);   // distance between slices

        std::vector<std::vector<int>> imageStack; // stack of images, each image is a vector of RGBA integers    
        for (int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex) {
            double z = -((numSlices / 2) * sliceThickness) + sliceIndex * sliceThickness;
            std::vector<int> image; // single image as a vector of RGB integers
            for (int yIndex = 0; yIndex < imageHeight; ++yIndex) {
                double y = -((imageHeight / 2) * pixelHeight) + yIndex * pixelHeight;
                for (int xIndex = 0; xIndex < imageWidth; ++xIndex) {
                    double x = -((imageWidth / 2) * pixelWidth) + xIndex * pixelWidth   ;
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

        // Construct the client actions
        for(int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex) {
            double z = -((numSlices / 2) * sliceThickness) + sliceIndex * sliceThickness;
            auto& image = imageStack[sliceIndex];
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
            actions.push_back(ActionSpec{"addGPlane", payload});
        }
    }
};


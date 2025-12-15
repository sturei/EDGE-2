#include <nlohmann/json.hpp>
#include "brep/body.h"
#include "utils/vec3d.h"
#include "brep/navigate.h"
#include "brep/tessellate.h"
#include "frep/functions.h"
#include "document/document.h"
#include "shape/shapeModel.h"

using json = nlohmann::json;

/**
 * Scene Actions are dispatched by action routines to keep the graphics scene in sync with the models.
 * The graphics scene is in fact managed in the client application. 
 * Hence these actions are so-called "client actions".
 * This module provides utilities to construct the payloads for those actions.
 */

namespace e2 {

    void dispatchGraphicsActionsForAcorn(Document& doc, const Body& acornBody) {
        // The payload is a point
        const Vec3d& position = acornBody.cell(0).support().position();
        json payload = json::object({{"position", json::array({position.x(), position.y(), position.z()})}});
        doc.dispatchClientAction({"Gfx::addPoint", payload});
    }

    void dispatchGraphicsActionsForSketch(Document& doc, const Body& sketchBody) {
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

    void dispatchGraphicsActionsForProfile(Document& doc, const Body& profileBody) {
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

    void dispatchGraphicsActionsForObject(Document& doc, const FObject& fobject, double width, double height, double depth) {

        // Generate a stack of images representing the SDF of the given object
        
        int numSlabsX = 1;       // only one slab in X and Y for now, until "z" is replaced by "position" in the addPlane action (to allow placing the slabs arbitrarily in XY)
        int numSlabsY = 1;

        // Choose image dimensions to give about 5 texels per world unit
        int imageWidth = width * 5 / numSlabsX;
        int imageHeight = height * 5 / numSlabsY;

        int numSlices = 5;

        double pixelWidth = width / (imageWidth * numSlabsX);     // size of each texel in world units
        double pixelHeight = height / (imageHeight * numSlabsY);  // size of each texel in world units
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
                            bool evalResult = fobject.evaluate(position, sdfValue);
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

    static void ensureProductParentsExist(Document& doc) {
        // Ensure parent items exists in the client
        doc.dispatchClientAction({"Gfx::addProductItem", json::object({
            {"pathName", "shape/workplanes"},
            {"displayName", "workplanes"}
        })});
        doc.dispatchClientAction({"Gfx::addProductItem", json::object({
            {"pathName", "shape/profiles"},
            {"displayName", "profiles"}
        })});
        doc.dispatchClientAction({"Gfx::addProductItem", json::object({
            {"pathName", "shape/features"},
            {"displayName", "features"}
        })});
    }

    static void ensureObjectParentsExist(Document& doc) {

        // Ensure parent items exists in the client
    
        std::cerr << "ensuring object parents exist" << std::endl;      // ---DEBUG---    

        const Store& store = doc.storeAt("shape");
        const FeatureModel& features = dynamic_cast<const ShapeModel*>(store.model())->features();

        std::cerr << "got features " << features << std::endl;      // ---DEBUG---

        int numBlanks = 0;
        int numTools = 0;
        for (Feature* f : features.features()) {
            if (!f) {
                continue;
            }
            if (dynamic_cast<const Profile*>(f)) {
                continue;   // skip 2D features (profiles) - those are represented as planar meshes, not SDF nodes
            }
            if (f->featureEffect() == FeatureEffect::ADD) {
                numBlanks++;
            }
            if (f->featureEffect() == FeatureEffect::SUBTRACT) {    
                numTools++;
            }   
        }
        
        std::cerr << "numBlanks: " << numBlanks << ", numTools: " << numTools << std::endl;      // ---DEBUG---

        if (numBlanks + numTools > 0) {
            // add the root node: "objects"
            doc.dispatchClientAction({"Gfx::addSdfNode", json::object({
                {"pathName", "objects"},
                {"type", "intersection"}
            })});
        }

        if (numBlanks > 0) {
            // add the "blanks" node to hold all the additive features
            doc.dispatchClientAction({"Gfx::addSdfNode", json::object({
                {"pathName", "objects/blanks"},
                {"type", "union"}
            })});
        }

        if (numTools > 0) {
            // add the "tools/tools" node to hold all the subtractive features
            doc.dispatchClientAction({"Gfx::addSdfNode", json::object({
                {"pathName", "objects/tools"},
                {"type", numBlanks == 0 ? "union" : "complement"}     // if there are no blanks, union the tools so we can see them
            })});
            doc.dispatchClientAction({"Gfx::addSdfNode", json::object({
                {"pathName", "objects/tools/tools"},
                {"type", "union"}
            })});
        }
    }

    void dispatchGraphicsActionsForScene(Document& doc) {

        ensureObjectParentsExist(doc);

        // hard-coded for now

        doc.dispatchClientAction({"Gfx::addSdfNode", json::object({
            {"pathName", "objects/blanks/block1"},
            {"type", "block"},
            {"width", 10.0},
            {"height", 10.0},
            {"depth", 5.0}
        })});

        doc.dispatchClientAction({"Gfx::updateSdfScene", json::object({})});

    }

    void dispatchProductActionsForBody(Document& doc, const Body& body, size_t bodyIndex, const std::string& bodyType, const std::string& parentPathName) {

        ensureProductParentsExist(doc);

        // Add an item for the body
        std::string bodyPathName = parentPathName + "/" + "body[" + std::to_string(bodyIndex) + "]";
        std::string bodyDisplayName = "body[" + std::to_string(bodyIndex) + "] " + " (" + bodyType + ")";
        json bodyPayload = json::object({
            {"pathName", bodyPathName},
            {"displayName", bodyDisplayName}
        });
        doc.dispatchClientAction({"Gfx::addProductItem", bodyPayload});

        // TODO: add items for cells within the body?
        
    }

    void dispatchProductActionsForObject(Document& doc, const FObject& fobject, size_t objectIndex, const std::string& objectType, const std::string& parentPathName) {

        ensureProductParentsExist(doc);

        // Add an item for the object
        std::string objectPathName = parentPathName + "/" + "body[" + std::to_string(objectIndex) + "]";
        std::string objectDisplayName = "object[" + std::to_string(objectIndex) + "] " + " (" + objectType + ")";
        json objectPayload = json::object({
            {"pathName", objectPathName},
            {"displayName", objectDisplayName}
        });
        doc.dispatchClientAction({"Gfx::addProductItem", objectPayload});

        // TODO: add items for fnodes within the object (but maybe need to invent features first)
        
    }

    void dispatchProductActionsForNewFeature(Document& doc, size_t featureIndex) {

        //
        // Features are listed in the viewer's product tree under "shape/features" or "shape/profiles" for 3D and 2D features (a.k.a. profiles) respectively.
        // 

        // For now, clear the product items and rebuild from scratch (could be optimized later)
        doc.dispatchClientAction({"Gfx::clearProductItems", json::object({})});  

        // Ensure parent items, especially "shape/features" and "shape/profiles" exist
        ensureProductParentsExist(doc);
        
        // Add an item for each feature
        const FeatureModel& features = dynamic_cast<const ShapeModel*>(doc.storeAt("shape").model())->features();
        for (size_t featureIndex = 0; featureIndex < features.numFeatures(); ++featureIndex) {  
            const Feature& feature = features.feature(featureIndex);
            std::string featurePathName = feature.pathname();
            std::string featureDisplayName = "";
            featureDisplayName += " " + feature.displayEffect();
            featureDisplayName += " " + feature.displayName();
            featureDisplayName += " (" + feature.displayType() + ")";

            json featurePayload = json::object({
                {"pathName", featurePathName},
                {"displayName", featureDisplayName}
            });

            doc.dispatchClientAction({"Gfx::addProductItem", featurePayload});
        }
    }

    // TOGO
    static void dispatchGraphicsActionsForSdfProfile(Document& doc, const std::string& pathName, const Body& profileBody) {
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

        json payload = json::object({
            {"pathName", pathName},
            {"type", "profile"},
            {"paths", paths}
        });
        doc.dispatchClientAction({"Gfx::addSdfNode", payload});
    }

    void dispatchGraphicsActionsForNewProfile(Document& doc, size_t profileIndex) {

        //
        // Profiles are represented graphically in the viewer as planar meshes.
        //

        // TODO: rebuild from scratch (could be optimized later) - so that refreshing the browser, or using multiple browsers against the same service, stays more or less in sync. (Future - do it better!

        const BRepModel& profiles = dynamic_cast<const ShapeModel*>(doc.storeAt("shape").model())->profiles();
        const Body& profileBody = *profiles.body(profileIndex);

        // The payload is a collection of "paths", representing the outline of the profile body. 
        // Paths are correctly oriented, but need not be contiguous.

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
        doc.dispatchClientAction({"Gfx::addContour", payload});
    }

    static void addSdfNodeForFeature(Document& doc, const Feature* feature, std::string objectPathName) {
        const FeatureModel& features = dynamic_cast<const ShapeModel*>(doc.storeAt("shape").model())->features();
        
        // TODO: handle position and rotation

        if (const Block* blockFeature = dynamic_cast<const Block*>(feature)) {
            double width = blockFeature->width();
            double height = blockFeature->height();
            double depth = blockFeature->depth();
            json featurePayload = json::object({
                {"pathName", objectPathName},
                {"type", "block"},
                {"width", width},
                {"height", height},
                {"depth", depth}
            });
            doc.dispatchClientAction({"Gfx::addSdfNode", featurePayload});
        }
        else if (const Sphere* sphereFeature = dynamic_cast<const Sphere*>(feature)) {
            double radius = sphereFeature->radius();
            json featurePayload = json::object({
                {"pathName", objectPathName},
                {"type", "sphere"},
                {"radius", radius}
            });
            doc.dispatchClientAction({"Gfx::addSdfNode", featurePayload});
        }
        else if (const Cylinder* cylinderFeature = dynamic_cast<const Cylinder*>(feature)) {
            double radius = cylinderFeature->radius();
            double depth = cylinderFeature->depth();
            json featurePayload = json::object({
                {"pathName", objectPathName},
                {"type", "cylinder"},
                {"radius", radius},
                {"depth", depth}
            });
            doc.dispatchClientAction({"Gfx::addSdfNode", featurePayload});
        }
        else if (const Rectangle2D* rectangle2DFeature = dynamic_cast<const Rectangle2D*>(feature)) {
            double width = rectangle2DFeature->width();
            double height = rectangle2DFeature->height();
            json featurePayload = json::object({
                {"pathName", objectPathName},
                {"type", "rectangle2D"},
                {"width", width},
                {"height", height}
            });
            doc.dispatchClientAction({"Gfx::addSdfNode", featurePayload});
        }
        else if (const Circle2D* circleFeature = dynamic_cast<const Circle2D*>(feature)) {
            double radius = circleFeature->radius();
            json featurePayload = json::object({
                {"pathName", objectPathName},
                {"type", "circle"},
                {"radius", radius}
            });
            doc.dispatchClientAction({"Gfx::addSdfNode", featurePayload});
        }
        else if (const RoundRect2D* roundRectFeature = dynamic_cast<const RoundRect2D*>(feature)) {
            double width = roundRectFeature->width();
            double height = roundRectFeature->height();
            double cornerRadius = roundRectFeature->cornerRadius();
            json featurePayload = json::object({
                {"pathName", objectPathName},
                {"type", "roundRect"},
                {"width", width},
                {"height", height},
                {"cornerRadius", cornerRadius}
            });
            doc.dispatchClientAction({"Gfx::addSdfNode", featurePayload});
        }
        else if (const Extrusion* extrusionFeature = dynamic_cast<const Extrusion*>(feature)) {
            double depth = extrusionFeature->depth();

            // to avoid z-fighting when subtracting. TODO: think of a better way!
            double epsilon = extrusionFeature->featureEffect() == FeatureEffect::SUBTRACT ? 0.01 : 0.0;
            bool doubleSided = extrusionFeature->doubleSided();

            json transformPayload = json::object({
                {"pathName", objectPathName},
                {"type", "translation"},
                {"translation", json::array({0.0, 0.0, doubleSided ? 0.0 : depth / 2.0})}
            });

            objectPathName += "/extrusion";
            json featurePayload = json::object({
                {"pathName", objectPathName},
                {"type", "extrusion"},
                {"depth", depth+epsilon}
            });

            std::string profilePathName = extrusionFeature->profilePathName();
            Feature* profile = features.findFeature(profilePathName);
            if (profile) {
                doc.dispatchClientAction({"Gfx::addSdfNode", transformPayload});
                doc.dispatchClientAction({"Gfx::addSdfNode", featurePayload});
                // add the profile feature as a child, recursively
                addSdfNodeForFeature(doc, profile, objectPathName + "/profile");
            }
        }        
    }

    void dispatchGraphicsActionsForNewFeature(Document& doc, size_t featureIndex) {

        // 3D Features are represented graphically in the viewer as the f=0 level set of a signed distance function (SDF).

        // Each feature maps to an SDF node, that may in turn contain child nodes.
        // By design, all the subtractive features (a.k.a. "tools") are subtracted from all the additive features (a.k.a. "blanks") to form the final shape.
        // This formulation has some advantages:
        // 1. features are order-independent
        //    - features can be reordered for better understanding
        //    - tools can be added before blanks if desired
        //    - features can be suppressed or filtered out without breaking other features
        //    - features can be added by several users simultaneously without conflict

        // For now, clear the scene and rebuild it from scratch (could be optimized later)
        doc.dispatchClientAction({"Gfx::clearSdfScene", json::object({})});

        // ensure parent items "objects", "objects/blanks", "objects/tools/tools" (if needed) exist
        ensureObjectParentsExist(doc);
        
        // Build the SDF nodes for each feature and add to the scene
        const FeatureModel& features = dynamic_cast<const ShapeModel*>(doc.storeAt("shape").model())->features();
        for (size_t featureIndex = 0; featureIndex < features.numFeatures(); ++featureIndex) {  
            const Feature& feature = features.feature(featureIndex);
            std::string objectPathName = "objects/";

            if (dynamic_cast<const Profile*>(&feature)) {
                // skip 2D features (profiles) - those are represented as planar meshes, not SDF nodes
                continue;
            } 
            else if (feature.featureEffect() == FeatureEffect::ADD) {
                objectPathName += "blanks/";
            } else if (feature.featureEffect() == FeatureEffect::SUBTRACT) {
                objectPathName += "tools/tools/";
            } else {
                // TODO: MODIFY features. We currently do not add anything to the graphics scene for them
                continue;
            }

            // Append the feature index to the object path name to make it unique
            objectPathName += "feature[" + std::to_string(featureIndex) + "]";

            // Now add the SDF node for this feature
            addSdfNodeForFeature(doc, &feature, objectPathName);

        }

        // Finally, update the SDF scene
        doc.dispatchClientAction({"Gfx::updateSdfScene", json::object({})});
    }
};


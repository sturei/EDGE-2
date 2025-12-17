#include <nlohmann/json.hpp>
#include "brep/body.h"
#include "utils/vec3d.h"
#include "brep/navigate.h"
#include "brep/tessellate.h"
#include "document/document.h"
#include "shape/shapeModel.h"

using json = nlohmann::json;

/**
 * Scene Actions are dispatched by action routines to keep the graphics scene (in the viewer) in sync with the model (in the server).
 * The graphics scene consists of the following:
 * - A product hierarchy tree, representing the features and profiles in the model.
 * - A set of drawables, representing the profiles in the model.
 * - A set of SDF nodes, representing the 3D features in the model.
 */

namespace e2 {

    // Ensure that the client has the necessary parent items in the product tree
    static void ensureProductParentsExist(Document& doc) {
        doc.dispatchClientAction({"Gfx::addProductItem", json::object({
            {"pathName", "shape/profiles"},
            {"displayName", "profiles"}
        })});
        doc.dispatchClientAction({"Gfx::addProductItem", json::object({
            {"pathName", "shape/features"},
            {"displayName", "features"}
        })});
    }

    // Ensure that the client has the necessary parent SDF nodes
    static void ensureSdfNodeParentsExist(Document& doc) {

        // Ensure parent items exists in the client
    
        std::cerr << "ensuring sdf node parents exist" << std::endl;      // ---DEBUG---    

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

    // Ensures that the product tree in the client is updated to reflect the features in the model
    void dispatchProductActionsForNewFeature(Document& doc, size_t featureIndex) {

        //
        // Features are listed in the viewer's product tree under "shape/features" or "shape/profiles" for 3D and 2D features respectively.
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

    // Ensures that the graphics scene in the client is updated to reflect the profiles in the model
    void dispatchGraphicsActionsForNewProfile(Document& doc, size_t profileIndex) {

        //
        // Profiles are represented graphically in the viewer as planar wireframe.
        //

        // For now, clear the drawables and rebuild from scratch (could be optimized later)
        doc.dispatchClientAction({"Gfx::clearDrawables", json::object({})});  

        const BRepModel& profiles = dynamic_cast<const ShapeModel*>(doc.storeAt("shape").model())->profiles();
        for (size_t profileIndex = 0; profileIndex < profiles.numBodies(); ++profileIndex) {
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

            const Tfm3d& tfm3d = profiles.transform(profileIndex);
            const Vec3d& p = tfm3d.position();
            const Vec3d& r = tfm3d.angles();
            json payload = json::object({
                {"paths", paths},
                {"position", json::array({p.x(), p.y(), p.z()})},
                {"rotation", json::array({r.x(), r.y(), r.z()})}
            });

            doc.dispatchClientAction({"Gfx::addContour", payload});
        }
    }

    // Utility to add an SDF node for a given feature
    static void addSdfNodeForFeature(Document& doc, const Feature* feature, std::string objectPathName) {
        const FeatureModel& features = dynamic_cast<const ShapeModel*>(doc.storeAt("shape").model())->features();
        
        if (const Block* blockFeature = dynamic_cast<const Block*>(feature)) {
            double width = blockFeature->width();
            double height = blockFeature->height();
            double depth = blockFeature->depth();
            json featureNode = json::object({
                {"pathName", objectPathName},
                {"type", "block"},
                {"width", width},
                {"height", height},
                {"depth", depth}
            });
            doc.dispatchClientAction({"Gfx::addSdfNode", featureNode});
        }
        else if (const Sphere* sphereFeature = dynamic_cast<const Sphere*>(feature)) {
            double radius = sphereFeature->radius();
            json featureNode = json::object({
                {"pathName", objectPathName},
                {"type", "sphere"},
                {"radius", radius}
            });
            doc.dispatchClientAction({"Gfx::addSdfNode", featureNode});
        }
        else if (const Cylinder* cylinderFeature = dynamic_cast<const Cylinder*>(feature)) {
            double radius = cylinderFeature->radius();
            double depth = cylinderFeature->depth();
            json featureNode = json::object({
                {"pathName", objectPathName},
                {"type", "cylinder"},
                {"radius", radius},
                {"depth", depth}
            });
            doc.dispatchClientAction({"Gfx::addSdfNode", featureNode});
        }
        else if (const Rectangle2D* rectangle2DFeature = dynamic_cast<const Rectangle2D*>(feature)) {
            double width = rectangle2DFeature->width();
            double height = rectangle2DFeature->height();
            json featureNode = json::object({
                {"pathName", objectPathName},
                {"type", "rectangle2D"},
                {"width", width},
                {"height", height}
            });
            doc.dispatchClientAction({"Gfx::addSdfNode", featureNode});
        }
        else if (const Circle2D* circleFeature = dynamic_cast<const Circle2D*>(feature)) {
            double radius = circleFeature->radius();
            json featureNode = json::object({
                {"pathName", objectPathName},
                {"type", "circle"},
                {"radius", radius}
            });

            doc.dispatchClientAction({"Gfx::addSdfNode", featureNode});
        }
        else if (const RoundRect2D* roundRectFeature = dynamic_cast<const RoundRect2D*>(feature)) {
            double width = roundRectFeature->width();
            double height = roundRectFeature->height();
            double cornerRadius = roundRectFeature->cornerRadius();
            json featureNode = json::object({
                {"pathName", objectPathName},
                {"type", "roundRect"},
                {"width", width},
                {"height", height},
                {"cornerRadius", cornerRadius}
            });
            doc.dispatchClientAction({"Gfx::addSdfNode", featureNode});
        }
        else if (const Extrusion* extrusionFeature = dynamic_cast<const Extrusion*>(feature)) {
            double depth = extrusionFeature->depth();
            const std::string profilePathName = extrusionFeature->profilePathName();
            Feature* profile = features.findFeature(profilePathName);
            if (!profile) {
                std::cerr << "Warning: Extrusion feature at " << objectPathName << " references missing profile at " << extrusionFeature->profilePathName() << std::endl;
                return;
            }

            // to avoid z-fighting when subtracting. TODO: think of a better way!
            double epsilon = extrusionFeature->featureEffect() == FeatureEffect::SUBTRACT ? 0.01 : 0.0;
            bool doubleSided = extrusionFeature->doubleSided();

            const Vec3d& profilePosition = profile->position();
            json profileTranslationNode = json::object({
                {"pathName", objectPathName},
                {"type", "translation"},
                {"position", json::array({profilePosition.x(), profilePosition.y(), profilePosition.z()})}
            });
            
            objectPathName += "/profileRotation";
            const Vec3d& profileRotation = profile->rotation();
            json profileRotationNode = json::object({
                {"pathName", objectPathName},
                {"type", "rotation"},
                {"angles", json::array({profileRotation.x(), profileRotation.y(), profileRotation.z()})}
            });

            objectPathName += "/zOffset";
            json zOffsetNode = json::object({
                {"pathName", objectPathName},
                {"type", "translation"},
                {"position", json::array({0.0, 0.0, doubleSided ? 0.0 : depth / 2.0})}
            });

            objectPathName += "/feature";
            json featureNode = json::object({
                {"pathName", objectPathName},
                {"type", "extrusion"},
                {"depth", depth+epsilon}
            });

            doc.dispatchClientAction({"Gfx::addSdfNode", profileTranslationNode});
            doc.dispatchClientAction({"Gfx::addSdfNode", profileRotationNode});
            doc.dispatchClientAction({"Gfx::addSdfNode", zOffsetNode});
            doc.dispatchClientAction({"Gfx::addSdfNode", featureNode});
            addSdfNodeForFeature(doc, profile, objectPathName + "/profile");
        }        
    }

    // Utility to add tranformation nodes for a given feature
    static void addSdfNodeForTransformedFeature(Document& doc, const Feature* feature, std::string objectPathName) {
        const FeatureModel& features = dynamic_cast<const ShapeModel*>(doc.storeAt("shape").model())->features();
        
        const Vec3d& position = feature->position();
        json translationNode = json::object({
            {"pathName", objectPathName},
            {"type", "translation"},
            {"position", json::array({position.x(), position.y(), position.z()})}
        });
        doc.dispatchClientAction({"Gfx::addSdfNode", translationNode});

        const Vec3d& rotation = feature->rotation();
        objectPathName += "/rotation";
        json rotationNode = json::object({
            {"pathName", objectPathName},
            {"type", "rotation"},
            {"angles", json::array({rotation.x(), rotation.y(), rotation.z()})}
        }); 
        doc.dispatchClientAction({"Gfx::addSdfNode", rotationNode});

        addSdfNodeForFeature(doc, feature, objectPathName + "/feature");
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
        ensureSdfNodeParentsExist(doc);
        
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
            addSdfNodeForTransformedFeature(doc, &feature, objectPathName);

        }

        // Finally, update the SDF scene
        doc.dispatchClientAction({"Gfx::updateSdfScene", json::object({})});
    }
};


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
            {"pathName", "shape/workplanes"},
            {"displayName", "workplanes"}
        })});
        doc.dispatchClientAction({"Gfx::addProductItem", json::object({
            {"pathName", "shape/features"},
            {"displayName", "features"}
        })});
    }

    // Ensure that the client has the necessary parent SDF nodes
    static void ensureSdfNodeParentsExist(Document& doc) {

        // Ensure parent items exists in the client
    
        const Store& store = doc.storeAt("shape");
        const FeatureModel& features = dynamic_cast<const ShapeModel*>(store.model())->features();

        std::cerr << "Ensuring sdf node parents exist for features: " << features << std::endl;      // ---DEBUG---

        int numBlanks = 0;
        int numTools = 0;
        for (Feature* f : features.features()) {
            if (!f) {
                continue;
            }
            if (dynamic_cast<const Feature2D*>(f) || dynamic_cast<const Workplane*>(f)) {
                // skip 2D features and workplanes. 
                // TODO: rather than relying on type, it would be better to have a method that returned the 
                // dimensionality of the feature, and count blanks and tools iff dim==3
                continue;
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
    static void dispatchProductActionsForNewFeature(Document& doc, size_t featureIndex) {

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
    static void dispatchGraphicsActionsForNewProfile(Document& doc, size_t profileIndex) {

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

            double zOffset = 0.0;
            const Attribute* attr = nullptr;
            const ProfileAttribute* profileAttr = nullptr;
            std::string color = "green";
            if (profileBody.findBodyAttribute("profileAttribute", attr) && (profileAttr = dynamic_cast<const ProfileAttribute*>(attr))) {
                zOffset = profileAttr->zOffset();
                color = profileAttr->isConstruction() ? "gray" : "green";
            }
            const Tfm3d& tfm3d = profiles.transform(profileIndex);
            const Vec3d& p = tfm3d.position();
            const Vec3d& r = tfm3d.angles();

            json payload = json::object({
                {"paths", paths},
                {"position", json::array({p.x(), p.y(), p.z()})},
                {"rotation", json::array({r.x(), r.y(), r.z()})},
                {"zOffset", zOffset},
                {"color", color}
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

            Feature* workplane = features.findFeature(dynamic_cast<Feature2D*>(profile)->workplanePathName());
            if (!workplane) {
                std::cerr << "Warning: Extrusion feature at " << objectPathName << " references profile at " << extrusionFeature->profilePathName() 
                          << " which references missing workplane at " << dynamic_cast<Feature2D*>(profile)->workplanePathName() << std::endl;
                return;
            }

            // to avoid z-fighting when subtracting. TODO: small offset would be a better way!
            double epsilon = extrusionFeature->featureEffect() == FeatureEffect::SUBTRACT ? 0.01 : 0.0;
            bool doubleSided = extrusionFeature->doubleSided();

            // TODO: take into account the 2D transformations on the profile

            const Vec3d& workplanePosition = workplane->position();
            json profileTranslationNode = json::object({
                {"pathName", objectPathName},
                {"type", "translation"},
                {"position", json::array({workplanePosition.x(), workplanePosition.y(), workplanePosition.z()})}
            });
            
            objectPathName += "/profileRotation";
            const Vec3d& profileRotation = workplane->rotation();
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

            // TODO: figure out where fill, offset, twist etc go
            doc.dispatchClientAction({"Gfx::addSdfNode", profileTranslationNode});
            doc.dispatchClientAction({"Gfx::addSdfNode", profileRotationNode});
            // probably fill goes in here
            doc.dispatchClientAction({"Gfx::addSdfNode", zOffsetNode});
            doc.dispatchClientAction({"Gfx::addSdfNode", featureNode});
            addSdfNodeForFeature(doc, profile, objectPathName + "/profile");
        }        
    }

    // Finds any modify feature that target the given feature
    static std::vector<const Feature*> findModifyingFeatures(const FeatureModel& features, const Feature* targetFeature) {
        std::vector<const Feature*> modifyingFeatures;
        for (const Feature* feature : features.features()) {
            if (feature->featureEffect() == FeatureEffect::MODIFY) {
                if (const Fill* fillFeature = dynamic_cast<const Fill*>(feature)) {
                    if (fillFeature->targetPathName() == targetFeature->pathname()) {
                        modifyingFeatures.push_back(feature);
                    }
                }
            }
        }
        return modifyingFeatures;
    }

    // Utility to add modification nodes for a given feature
    static void addSdfNodeForModifiedFeature(Document& doc, const Feature* targetFeature, std::string objectPathName) {
        const FeatureModel& features = dynamic_cast<const ShapeModel*>(doc.storeAt("shape").model())->features();

        std::vector<const Feature*> modifyingFeatures = findModifyingFeatures(features, targetFeature);
        if (modifyingFeatures.empty()) {
            // no modifying features - just add the feature as is
            addSdfNodeForFeature(doc, targetFeature, objectPathName);
            return;
        }
        
        // Currently, only Fill features modify other features
        if (const Fill* fillFeature = dynamic_cast<const Fill*>(modifyingFeatures[0])) {
            const std::string targetPathName = fillFeature->targetPathName();

            FillType fillType = fillFeature->fillType();
            double cellSize = fillFeature->cellSize();

            json featureNode = json::object({
                {"pathName", objectPathName},
                {"type", "intersection"},
            });
            doc.dispatchClientAction({"Gfx::addSdfNode", featureNode});

            if (fillType == FillType::SPHERE) {
            
                json repetitionNode = json::object({
                    {"pathName", objectPathName + "/repetition"},
                    {"type", "repetition"},
                    {"cellSize", cellSize}
                });

                json cellNode = json::object({
                    {"pathName", objectPathName + "/repetition/cell"},
                    {"type", "sphere"},
                    {"radius", cellSize / 3.0}              /// hard-coded for now - could be a parameter of the Fill feature
                });

                doc.dispatchClientAction({"Gfx::addSdfNode", repetitionNode});
                doc.dispatchClientAction({"Gfx::addSdfNode", cellNode});
            }
            else if (fillType == FillType::GYROID) { 
                json gyroidNode = json::object({
                    {"pathName", objectPathName + "/gyroid"},
                    {"type", "gyroid"},
                    {"cellSize", cellSize}
                });

                doc.dispatchClientAction({"Gfx::addSdfNode", gyroidNode});
            }

            addSdfNodeForFeature(doc, targetFeature, objectPathName + "/targetFeature");

        }
        else {
            addSdfNodeForFeature(doc, targetFeature, objectPathName);   
        }
    }

    // Utility to add transformation nodes for a given feature
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

        addSdfNodeForModifiedFeature(doc, feature, objectPathName + "/feature");
    }

    static void dispatchGraphicsActionsForNewFeature(Document& doc, size_t featureIndex) {

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

            if (dynamic_cast<const Feature2D*>(&feature) || dynamic_cast<const Workplane*>(&feature)) {
                // skip 2D features - TODO: add feature dimensionality method rather than relying on type
                continue;
            } 
            else if (feature.featureEffect() == FeatureEffect::ADD) {
                objectPathName += "blanks/";
            } else if (feature.featureEffect() == FeatureEffect::SUBTRACT) {
                objectPathName += "tools/tools/";
            } else {
                // skip MODIFY feature - those are processed when their target feature gets processed.
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

    void dispatchGraphicsActionsForModifiedScene(Document& doc) {
        // For now, just rebuild the entire graphics scene from scratch
        dispatchProductActionsForNewFeature(doc, 0);    // productIndex is ignored in the current implementation
        dispatchGraphicsActionsForNewProfile(doc, 0);   // profileIndex is ignored in the current implementation
        dispatchGraphicsActionsForNewFeature(doc, 0);   // featureIndex is ignored in the current implementation
    }
};


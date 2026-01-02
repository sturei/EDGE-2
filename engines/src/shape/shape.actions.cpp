#include <nlohmann/json.hpp>
#include "brep/brep.fixtures.h"
#include "shape/shapeModel.h"
#include "shape/shape.actions.h"
#include "document/document.h"
#include "document/store.h"
#include "utils/vec3d.h"

using json = nlohmann::json;

/**
 * Shapes are made up of features.
 * 2D features have a dual representation - as a feature, and as a (2D) Brep. 
 * 3D features are represented eventually in the viewer as SDF Nodes.
 * 2D features are represented eventually in the viewer as Drawables (and sometimes also as an SDF Node within a 3D feature such as an extrusion, that depends on a 2D feature).
 * Both 2D and 3D features are also represented, eventually, as items in the product tree in the viewer.
 * See sceneActions.cpp for details of how the feature model and brep model are converted to viewer representations.
 * 3D features can be additive, subtractive or modifiers. Additive and subtractive features do not depend on other 3D features. 
 * They can be added in any order; and activated/deactivated independently. Also, many changes to the shape model can be made by modifying 2D features, and allowing the 3D features to 
 * update automatically. The intention is that the global properties of the shape can be driven by a "layout", expressed in terms of sketches, workplanes and dimensions between them.
 * Modifier features depend on other 3D features. Thus they must specify which feature they modify; and if that feature is deactivated, the modifier is also deactivated.
 */

namespace e2 {
    namespace ShapeActions {

        static std::pair<Vec3d, Vec3d> parsePositionRotationJson(const json& positionJson, const json& rotationJson) {
            Vec3d position = Vec3d(
                positionJson.at(0).get<double>(),
                positionJson.at(1).get<double>(),
                positionJson.at(2).get<double>()
            );
            Vec3d rotation = Vec3d(
                rotationJson.at(0).get<double>(),
                rotationJson.at(1).get<double>(),
                rotationJson.at(2).get<double>()
            );
            return std::make_pair(position, rotation);
        }

        static std::pair<Vec3d, double> parsePositionRotation2DJson(const json& positionJson, double rotationJson) {
            Vec3d position = Vec3d(
                positionJson.at(0).get<double>(),
                positionJson.at(1).get<double>(),
                0.0
            );
            return std::make_pair(position, rotationJson);
        }

        static FeatureEffect parseFeatureEffectString(const std::string& featureEffectStr) {
            if (featureEffectStr == "add") {
                return FeatureEffect::ADD;
            }
            else if (featureEffectStr == "subtract") {
                return FeatureEffect::SUBTRACT;
            }
            else {
                return FeatureEffect::MODIFY;
            }
        }
        
        static FillType parseFillTypeString(const std::string& fillTypeStr) {
            if (fillTypeStr == "sphere") {
                return FillType::SPHERE;
            }
            else if (fillTypeStr == "gyroid") {
                return FillType::GYROID;
            }
            else {
                return FillType::SPHERE;   // default
            }
        }

        // This action adds a primitive feature
        void addPrimitive(Document& doc, const json& payload) {
            Store& store = doc.storeAt("shape");

            // unpack the payload
            std::string pathName = payload.value("pathName", "shape/features/unnamedPrimitive");
            std::string displayName = payload.value("displayName", pathName.substr(pathName.find_last_of("/") + 1));
            std::string primitiveType = payload.value("primitiveType", "block");
            FeatureEffect featureEffect = parseFeatureEffectString(payload.value("featureEffect", "add"));
            std::pair<Vec3d, Vec3d> posRot3D = parsePositionRotationJson(
                payload.value("position", json::array({0,0,0})),
                payload.value("rotation", json::array({0,0,0}))
            );

            size_t featureIndex = -1;
            if (primitiveType == "block") {

                // unpack the payload
                double width = payload.value("width", 2.0);
                double height = payload.value("height", 2.0);
                double depth = payload.value("depth", 2.0);

                store.changeState([pathName, displayName, featureEffect, posRot3D, width, height, depth, &doc](Model* model) {
                    // update the feature model
                    FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                    Feature* blockFeature = new Block(pathName, displayName, featureEffect, posRot3D.first, posRot3D.second, width, height, depth);
                    size_t index = features.addFeature(blockFeature);
                    std::cerr << "added Block Primitive" << std::endl;      // ---LOGGING---
                });
            }
            else if (primitiveType == "sphere") {

                // unpack the payload
                double radius = payload.value("radius", 1.0);

                store.changeState([pathName, displayName, featureEffect, posRot3D, radius, &doc](Model* model) {
                    // update the feature model
                    FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                    Feature* sphereFeature = new Sphere(pathName, displayName, featureEffect, posRot3D.first, posRot3D.second, radius);
                    size_t index = features.addFeature(sphereFeature);
                    std::cerr << "added Sphere Primitive" << std::endl;      // ---LOGGING---
                });
            }
            else if (primitiveType == "cylinder") {

                // unpack the payload
                double radius = payload.value("radius", 1.0);
                double depth = payload.value("depth", 2.0);

                // update the feature model
                store.changeState([pathName, displayName, featureEffect, posRot3D, radius, depth, &doc](Model* model) {
                    // update the feature model
                    FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                    Feature* cylinderFeature = new Cylinder(pathName, displayName, featureEffect, posRot3D.first, posRot3D.second, radius, depth);
                    size_t index = features.addFeature(cylinderFeature);
                    std::cerr << "added Cylinder Primitive" << std::endl;      // ---LOGGING---
                });
            }
        }

        // This action adds a workplane feature
        void addWorkplane(Document& doc, const json& payload) {
            Store& store = doc.storeAt("shape");    
            // unpack the payload
            std::string pathName = payload.value("pathName", "shape/layout/unnamedWorkplane");
            std::string displayName = payload.value("displayName", pathName.substr(pathName.find_last_of("/") + 1));
            std::pair<Vec3d, Vec3d> posRot3D = parsePositionRotationJson(
                payload.value("position", json::array({0,0,0})),
                payload.value("rotation", json::array({0,0,0}))
            );
            store.changeState([pathName, displayName, posRot3D, &doc](Model* model) {
                
                // update the feature model
                FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                Feature* workplaneFeature = new Workplane(pathName, displayName, posRot3D.first, posRot3D.second);
                size_t index = features.addFeature(workplaneFeature);

                // update the brep model
                BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                Body* workplaneBody = BRepFixtures::workplaneIndicator(pathName, displayName);
                workplaneBody->attachBodyAttribute("workplaneAttribute", new WorkplaneAttribute());
                size_t profileIndex = profiles.addBody(workplaneBody, Tfm3d(posRot3D.first, posRot3D.second));  

                std::cerr << "added Workplane Feature" << std::endl;      // ---LOGGING---
            });
        }

        // This action adds a 2D Primitive feature
        void addPrimitive2D(Document& doc, const json& payload) {
            Store& store = doc.storeAt("shape");

            // unpack the payload
            std::string pathName = payload.value("pathName", "shape/workplanes/unnamedPrimitive");
            std::string displayName = payload.value("displayName", pathName.substr(pathName.find_last_of("/") + 1));
            std::string workplanePathName = pathName.substr(0, pathName.find_last_of('/'));
            std::string primitiveType = payload.value("primitiveType", "rectangle");
            std::pair<Vec3d, Vec3d> posRot3D = parsePositionRotationJson(
                payload.value("position", json::array({0,0,0})),
                payload.value("rotation", json::array({0,0,0}))
            );
            std::pair<Vec3d, double> posRot2D = parsePositionRotation2DJson(
                payload.value("position2D", json::array({0,0})),
                payload.value("rotation2D", 0));

            size_t featureIndex = -1;
            if (primitiveType == "rectangle") {

                // unpack the payload
                double width = payload.value("width", 3.0);
                double height = payload.value("height", 2.0);

                store.changeState([pathName, displayName, workplanePathName, posRot3D, posRot2D, width, height, &doc](Model* model) {

                    // 3d position/rotation is inherited from workplane if it exists, overriding any specified 3d position/rotation.
                    FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                    Feature* workplane = features.findFeature(workplanePathName);
                    Tfm3d tfm3d = workplane ? Tfm3d(workplane->position(), workplane->rotation()) : Tfm3d(posRot3D.first, posRot3D.second);

                    // update the feature model 
                    Feature* rectangleFeature = new Rectangle2D(
                        pathName, displayName, tfm3d.position(), tfm3d.angles(),
                        workplanePathName, posRot2D.first, posRot2D.second,
                        width, height);
                    size_t featureIndex = features.addFeature(rectangleFeature);

                    // update the brep model
                    BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                    Body* rectangleBody = BRepFixtures::rectangle2DSheet(pathName, displayName, width, height);
                    rectangleBody->attachBodyAttribute("profileAttribute", new ProfileAttribute(false, posRot2D.first, posRot2D.second, 0.0)); 
                    size_t profileIndex = profiles.addBody(rectangleBody, tfm3d);        

                    std::cerr << "added Rectangle2D Primitive" << std::endl;      // ---LOGGING---
                });
            }
            else if (primitiveType == "circle") {

                // unpack the payload
                double radius = payload.value("radius", 1.0);

                store.changeState([pathName, displayName, workplanePathName, posRot3D, posRot2D, radius, &doc](Model* model) {

                    // 3d position/rotation is inherited from workplane if it exists, overriding any specified 3d position/rotation.
                    FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                    Feature* workplane = features.findFeature(workplanePathName);
                    Tfm3d tfm3d = workplane ? Tfm3d(workplane->position(), workplane->rotation()) : Tfm3d(posRot3D.first, posRot3D.second);

                    // update the feature model
                    Feature* circleFeature = new Circle2D(
                        pathName, displayName, tfm3d.position(), tfm3d.angles(),
                        workplanePathName, posRot2D.first, posRot2D.second,
                        radius);
                    size_t featureIndex = features.addFeature(circleFeature);

                    // update the brep model
                    BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                    Body* circleBody = BRepFixtures::circle2DSheet(pathName, displayName, radius);
                    circleBody->attachBodyAttribute("profileAttribute", new ProfileAttribute(false, posRot2D.first, posRot2D.second, 0.0)); 
                    size_t profileIndex = profiles.addBody(circleBody, tfm3d); 

                    std::cerr << "added Circle2D Primitive" << std::endl;      // ---LOGGING---
                }); 
            }
            else if (primitiveType == "roundRect") {

                // unpack the payload
                double width = payload.value("width", 2.0);
                double height = payload.value("height", 2.0);
                double cornerRadius = payload.value("cornerRadius", 0.2);

                store.changeState([pathName, displayName, workplanePathName, posRot3D, posRot2D, width, height, cornerRadius, &doc](Model* model) {
                    
                    // 3d position/rotation is inherited from workplane if it exists, overriding any specified 3d position/rotation.
                    FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                    Feature* workplane = features.findFeature(workplanePathName);
                    Tfm3d tfm3d = workplane ? Tfm3d(workplane->position(), workplane->rotation()) : Tfm3d(posRot3D.first, posRot3D.second);

                    // update the feature model
                    Feature* roundRectFeature = new RoundRect2D(
                        pathName, displayName, tfm3d.position(), tfm3d.angles(),
                        workplanePathName, posRot2D.first, posRot2D.second,
                        width, height, cornerRadius);
                    size_t featureIndex = features.addFeature(roundRectFeature);

                    // update the brep model
                    BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                    Body* roundRectBody = BRepFixtures::roundRect2DSheet(pathName, displayName, width, height, cornerRadius);
                    roundRectBody->attachBodyAttribute("profileAttribute", new ProfileAttribute(false, posRot2D.first, posRot2D.second, 0.0)); 
                    size_t profileIndex = profiles.addBody(roundRectBody, tfm3d);

                    std::cerr << "added RoundRect2D Primitive" << std::endl;      // ---LOGGING---
               });
            }
        }

        // This action adds an extrusion feature
        void addExtrusion(Document& doc, const json& payload) {
            Store& store = doc.storeAt("shape");

            // unpack the payload
            std::string pathName = payload.value("pathName", "shape/features/unnamedExtrusion");
            std::string displayName = payload.value("displayName", pathName.substr(pathName.find_last_of("/") + 1));
            FeatureEffect featureEffect = parseFeatureEffectString(payload.value("featureEffect", "add"));
            std::pair<Vec3d, Vec3d> posRot3D = parsePositionRotationJson(
                payload.value("position", json::array({0,0,0})),
                payload.value("rotation", json::array({0,0,0}))
            );
            std::string profilePathName = payload.value("profilePathName", "shape/profiles/unnamedProfile");
            double depth = payload.value("depth", 1.0);
            bool doubleSided = payload.value("doubleSided", false);

            // update the model
            store.changeState([pathName, displayName, featureEffect, posRot3D, profilePathName, depth, doubleSided, &doc](Model* model) {
                
                // update the feature model
                FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                Feature* extrusionFeature = new Extrusion(
                    pathName, displayName, featureEffect, 
                    posRot3D.first, posRot3D.second, profilePathName,     // normally the 3D position/rotation comes from the workplane of the profile, but it can be overridden
                    depth, doubleSided);
                size_t index = features.addFeature(extrusionFeature);

                // update the brep model
                BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                size_t profileIndex;
                if (!profiles.findBodyIndex(profilePathName, profileIndex)) {
                    std::cerr << "Warning: could not find profile body for extrusion feature at path "<< profilePathName << std::endl;
                }
                else {
                    // add a copy of the profile body as construction geometry at the end(s) of the extrusion.
                    Body* profileBody = profiles.body(profileIndex);
                    Tfm3d tfm3d = profiles.transform(profileIndex);
                    
                    const Attribute* attr = nullptr;
                    Vec3d position2D;
                    double rotation2D = 0.0;
                    if (profileBody->findBodyAttribute("profileAttribute", attr)) {
                        const ProfileAttribute* profileAttr = dynamic_cast<const ProfileAttribute*>(attr);
                        if (profileAttr) {
                            position2D = profileAttr->position2D();
                            rotation2D = profileAttr->rotation2D();
                        }
                    }

                    double zOffset = doubleSided ? depth / 2.0 : depth;
                    bool isConstruction = true;         // the auto-generated profiles are created as construction geometry
                    if (doubleSided) {
                        // add at -depth/2
                        Body* startBody = new Body(*profileBody);
                        startBody->attachBodyAttribute("profileAttribute", new ProfileAttribute(isConstruction, position2D, rotation2D, -zOffset)); 
                        startBody->setPathName(profilePathName + "_start");
                        startBody->setDisplayName(displayName + " Start");
                        size_t startIndex = profiles.addBody(startBody, tfm3d);
                    }
                    
                    Body* endBody = new Body(*profileBody);
                    endBody->attachBodyAttribute("profileAttribute", new ProfileAttribute(isConstruction, position2D, rotation2D, zOffset));
                    endBody->setPathName(profilePathName + "_end");
                    endBody->setDisplayName(displayName + " End");
                    size_t endIndex = profiles.addBody(endBody, tfm3d);
                }       
                    
                std::cerr << "added Extrusion Feature" << std::endl;      // ---LOGGING---
            });     
        }

        // This action adds an fill feature
        void addFill(Document& doc, const json& payload) {
            Store& store = doc.storeAt("shape");

            // unpack the payload
            std::string pathName = payload.value("pathName", "shape/features/unnamedFill");
            std::string displayName = payload.value("displayName", pathName.substr(pathName.find_last_of("/") + 1));
            // right now it's always a "modify" at the feature level (it always has a target).
            // In future it could perhaps have options to a) be global and/or b) have a "cell effect"
            FeatureEffect featureEffect = FeatureEffect::MODIFY;        //parseFeatureEffectString(payload.value("featureEffect", "modify"));
            std::pair<Vec3d, Vec3d> posRot3D = parsePositionRotationJson(
                payload.value("position", json::array({0,0,0})),
                payload.value("rotation", json::array({0,0,0}))
            );
            std::string targetPathName = payload.value("targetPathName", "shape/profiles/unnamedTarget");
            double cellSize = payload.value("cellSize", 1.0);     // Future - make it an array for different sizes in different directions
            FillType fillType = parseFillTypeString(payload.value("fillType", "sphere")); 

            store.changeState([pathName, displayName, featureEffect, posRot3D, targetPathName, cellSize, fillType, &doc](Model* model) {
                // update the feature model
                FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                Feature* fillFeature = new Fill(pathName, displayName, featureEffect, posRot3D.first, posRot3D.second, targetPathName, fillType, cellSize);
                size_t index = features.addFeature(fillFeature);
                std::cerr << "added Fill Feature" << std::endl;      // ---LOGGING---
            });     
        }

        // This action does a no-op to trigger firing the client actions
        void triggerClientUpdate(Document& doc, const json& payload) {
            Store& store = doc.storeAt("shape");
            // update the model
            store.changeState([&doc](Model* _model) {
                // no-op change to trigger post-state-change callback
                std::cerr << "did a no-op on the shape store to trigger a client update" << std::endl;      // ---LOGGING---
            });       
        }

        // This action removes all the features from the shape model
        void clearAllFeatures(Document& doc, const json& payload) {
            Store& store = doc.storeAt("shape");
            store.changeState([&doc](Model* model) {
                //clear the feature model
                FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                features.clearFeatures();
                //clear the brep model
                BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                profiles.clearBodies();
                std::cerr << "cleared all features and breps from the shape model" << std::endl;      // ---LOGGING---    
            });
        }
    }
}   


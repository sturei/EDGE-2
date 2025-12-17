#include <nlohmann/json.hpp>
#include "brep/brep.fixtures.h"
#include "shape/shapeModel.h"
#include "document/document.h"
#include "document/store.h"
#include "utils/vec3d.h"
#include "scene/sceneActions.h"

using json = nlohmann::json;

/**
 * ShapeActions provides a set of actions that enable clients to create and manipulate shapes.
 */

namespace e2 {
    namespace ShapeActions {

        std::pair<Vec3d, Vec3d> parsePositionRotationJson(const json& positionJson, const json& rotationJson) {
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

        std::pair<Vec3d, double> parsePositionRotation2DJson(const json& positionJson, double rotationJson) {
            Vec3d position = Vec3d(
                positionJson.at(0).get<double>(),
                positionJson.at(1).get<double>(),
                0.0
            );
            return std::make_pair(position, rotationJson);
        }

        void addPrimitive(Document& doc, const json& payload) {
            // This action adds a primitive feature as a feature
            Store& store = doc.storeAt("shape");
            std::string pathName = payload.value("pathName", "shape/features/unnamedPrimitive");
            std::string displayName = payload.value("displayName", pathName.substr(pathName.find_last_of("/") + 1));
            std::string primitiveType = payload.value("primitiveType", "block");
            std::string featureEffect = payload.value("featureEffect", "add");
            FeatureEffect featureEffectEnum = featureEffect == "add" ? FeatureEffect::ADD :
                                            featureEffect == "subtract" ? FeatureEffect::SUBTRACT :
                                            FeatureEffect::MODIFY;
            std::pair<Vec3d, Vec3d> posRot3D = parsePositionRotationJson(
                payload.value("position", json::array({0,0,0})),
                payload.value("rotation", json::array({0,0,0}))
            );

            size_t featureIndex = -1;
            if (primitiveType == "block") {
                double width = payload.value("width", 2.0);
                double height = payload.value("height", 2.0);
                double depth = payload.value("depth", 2.0);

                store.changeState([pathName, displayName, featureEffectEnum, posRot3D, width, height, depth, &doc](Model* model) {
                    // update the model
                    FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                    Feature* blockFeature = new Block(pathName, displayName, featureEffectEnum, posRot3D.first, posRot3D.second, width, height, depth);
                    size_t index = features.addFeature(blockFeature);
                    std::cerr << "added Block Primitive" << std::endl;      // ---LOGGING---

                    // update the viewer
                    dispatchGraphicsActionsForModifiedScene(doc);
                });
            }
            else if (primitiveType == "sphere") {
                double radius = payload.value("radius", 1.0);

                store.changeState([pathName, displayName, featureEffectEnum, posRot3D, radius, &doc](Model* model) {
                    // update the model
                    FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                    Feature* sphereFeature = new Sphere(pathName, displayName, featureEffectEnum, posRot3D.first, posRot3D.second, radius);
                    size_t index = features.addFeature(sphereFeature);
                    std::cerr << "added Sphere Primitive" << std::endl;      // ---LOGGING---

                    // update the viewer
                    dispatchGraphicsActionsForModifiedScene(doc);
                });
            }
            else if (primitiveType == "cylinder") {
                double radius = payload.value("radius", 1.0);
                double depth = payload.value("depth", 2.0);
                store.changeState([pathName, displayName, featureEffectEnum, posRot3D, radius, depth, &doc](Model* model) {
                    // update the model
                    FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                    Feature* cylinderFeature = new Cylinder(pathName, displayName, featureEffectEnum, posRot3D.first, posRot3D.second, radius, depth);
                    size_t index = features.addFeature(cylinderFeature);
                    std::cerr << "added Cylinder Primitive" << std::endl;      // ---LOGGING---

                    // update the viewer
                    dispatchGraphicsActionsForModifiedScene(doc);

                });
            }
        }

        void addPrimitive2D(Document& doc, const json& payload) {
            // This action adds a primitive profile feature as a feature
            Store& store = doc.storeAt("shape");
            std::string pathName = payload.value("pathName", "shape/profiles/unnamedPrimitive");
            std::string displayName = payload.value("displayName", pathName.substr(pathName.find_last_of("/") + 1));
            std::string primitiveType = payload.value("primitiveType", "rectangle");
            std::string featureEffect = payload.value("featureEffect", "add");
            FeatureEffect featureEffectEnum = featureEffect == "add" ? FeatureEffect::ADD :
                                            featureEffect == "subtract" ? FeatureEffect::SUBTRACT :
                                            FeatureEffect::MODIFY;
            std::pair<Vec3d, Vec3d> posRot3D = parsePositionRotationJson(
                payload.value("position", json::array({0,0,0})),
                payload.value("rotation", json::array({0,0,0}))
            );
            std::pair<Vec3d, double> posRot2D = parsePositionRotation2DJson(
                payload.value("position2D", json::array({0,0})),
                payload.value("rotation2D", 0));

            size_t featureIndex = -1;
            if (primitiveType == "rectangle") {
                double width = payload.value("width", 3.0);
                double height = payload.value("height", 2.0);

                store.changeState([pathName, displayName, featureEffectEnum, posRot3D, posRot2D, width, height, &doc](Model* model) {
                    // update the model
                    FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                    Feature* rectangleFeature = new Rectangle2D(
                        pathName, displayName, featureEffectEnum, 
                        posRot3D.first, posRot3D.second, 
                        posRot2D.first, posRot2D.second,
                        width, height);
                    size_t featureIndex = features.addFeature(rectangleFeature);

                    BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                    Body* rectangleBody = BRepFixtures::rectangle2DSheet(width, height);     // TODO: take 2D position/rotation into account. Ditto for other 2D primitives
                    Vec3d position = posRot3D.first;

                    Tfm3d tfm3d(posRot3D.first, posRot3D.second);
                    size_t profileIndex = profiles.addBody(rectangleBody, tfm3d);        

                    std::cerr << "added Rectangle2D Primitive" << std::endl;      // ---LOGGING---

                    // update the viewer
                    dispatchGraphicsActionsForModifiedScene(doc);
                });
            }
            else if (primitiveType == "circle") {
                double radius = payload.value("radius", 1.0);

                store.changeState([pathName, displayName, featureEffectEnum, posRot3D, posRot2D, radius, &doc](Model* model) {
                    // update the model
                    FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                    Feature* circleFeature = new Circle2D(
                        pathName, displayName, featureEffectEnum, 
                        posRot3D.first, posRot3D.second, 
                        posRot2D.first, posRot2D.second,
                        radius);
                    size_t featureIndex = features.addFeature(circleFeature);

                    BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                    Body* circleBody = BRepFixtures::circle2DSheet(radius);

                    Tfm3d tfm3d(posRot3D.first, posRot3D.second);
                    size_t profileIndex = profiles.addBody(circleBody, tfm3d); 

                    std::cerr << "added Circle2D Primitive" << std::endl;      // ---LOGGING---

                    // update the viewer
                    dispatchGraphicsActionsForModifiedScene(doc);
                });
            }
            else if (primitiveType == "roundRect") {
                double width = payload.value("width", 2.0);
                double height = payload.value("height", 2.0);
                double cornerRadius = payload.value("cornerRadius", 0.2);
                store.changeState([pathName, displayName, featureEffectEnum, posRot3D, posRot2D, width, height, cornerRadius, &doc](Model* model) {
                    // update the model
                    FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                    Feature* roundRectFeature = new RoundRect2D(
                        pathName, displayName, featureEffectEnum,
                        posRot3D.first, posRot3D.second, 
                        posRot2D.first, posRot2D.second,
                        width, height, cornerRadius);
                    size_t featureIndex = features.addFeature(roundRectFeature);

                    BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                    Body* roundRectBody = BRepFixtures::roundRect2DSheet(width, height, cornerRadius);

                    Tfm3d tfm3d(posRot3D.first, posRot3D.second);
                    size_t profileIndex = profiles.addBody(roundRectBody, tfm3d);

                    std::cerr << "added RoundRect2D Primitive" << std::endl;      // ---LOGGING---

                    // update the viewer
                    dispatchGraphicsActionsForModifiedScene(doc);
               });
            }
        }

        void addExtrusion(Document& doc, const json& payload) {
            // This action adds an extrusion feature as a feature
            Store& store = doc.storeAt("shape");
            std::string pathName = payload.value("pathName", "shape/features/unnamedExtrusion");
            std::string displayName = payload.value("displayName", pathName.substr(pathName.find_last_of("/") + 1));
            std::string featureEffect = payload.value("featureEffect", "add");
            FeatureEffect featureEffectEnum = 
                featureEffect == "add" ? FeatureEffect::ADD :
                featureEffect == "subtract" ? FeatureEffect::SUBTRACT : FeatureEffect::MODIFY;
            std::pair<Vec3d, Vec3d> posRot3D = parsePositionRotationJson(
                payload.value("position", json::array({0,0,0})),
                payload.value("rotation", json::array({0,0,0}))
            );

            std::string profilePathName = payload.value("profilePathName", "shape/profiles/unnamedProfile");
            double depth = payload.value("depth", 1.0);
            bool doubleSided = payload.value("doubleSided", false);

            store.changeState([pathName, displayName, featureEffectEnum, posRot3D, profilePathName, depth, doubleSided, &doc](Model* model) {
                // update the model
                FeatureModel& features = dynamic_cast<ShapeModel*>(model)->features();
                Feature* extrusionFeature = new Extrusion(pathName, displayName, featureEffectEnum, posRot3D.first, posRot3D.second, profilePathName, depth, doubleSided);
                size_t index = features.addFeature(extrusionFeature);
                std::cerr << "added Extrusion Feature" << std::endl;      // ---LOGGING---

                // update the viewer
                dispatchGraphicsActionsForModifiedScene(doc);
            });     
        }
    }
};


#include <nlohmann/json.hpp>
#include "brep/brep.fixtures.h"
#include "frep/frep.fixtures.h"
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

        std::pair<Vec3d, Vec3d> parseLowerUpperJson(const json& llJson, const json& urJson) {
            Vec3d lowerLeft = Vec3d(
                llJson.at(0).get<double>(),
                llJson.at(1).get<double>(),
                llJson.at(2).get<double>()
            );
            Vec3d upperRight = Vec3d(
                urJson.at(0).get<double>(),
                urJson.at(1).get<double>(),
                urJson.at(2).get<double>()
            );
            return std::make_pair(lowerLeft, upperRight);
        }

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

        void addEmptyBody(Document& doc, const json& payload) {
            // This action adds an empty body (a body with no cells) to the brep store.
            Store& store = doc.storeAt("shape");
            store.changeState([](Model* model) {
                BRepModel& sketches = dynamic_cast<ShapeModel*>(model)->sketches();
                Body* emptyBody = BRepFixtures::emptyBody();
                sketches.addBody(emptyBody);
                std::cerr << "added Empty Body" << std::endl;      // ---LOGGING---
            });
        }

        void addAcornBody(Document& doc, const json& payload) {
            // This action adds an acorn body (a body with a single vertex) to the brep store.
            json position = payload.value("position", json::object({{"x", 0}, {"y", 0}, {"z", 0}}));
            Vec3d acornPosition = Vec3d(
                position.at("x").get<double>(),
                position.at("y").get<double>(),
                position.at("z").get<double>()
            );

            Store& store = doc.storeAt("shape");
            store.changeState([acornPosition, &doc](Model* model) {

                // update the model
                BRepModel& sketches = dynamic_cast<ShapeModel*>(model)->sketches();
                Body* acornBody = BRepFixtures::acornBody(acornPosition);
                size_t index = sketches.addBody(acornBody);
                std::cerr << "added Acorn Body" << std::endl;      // ---LOGGING--- 

                // update the product hierarchy in the client
                dispatchProductActionsForBody(doc, *acornBody, index, "acorn", "shape/workplanes");
                // update the scene in the client
                dispatchGraphicsActionsForAcorn(doc, *acornBody);
            });
        }

        void addWireRectangle(Document& doc, const json& payload) {
            // This action adds a wire rectangle as a sketch.
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::array({0,0,0})),
                payload.value("upperRight", json::array({1,1,0}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;

            Store& store = doc.storeAt("shape");
            store.changeState([lowerLeft, upperRight, &doc](Model* model) {

                // update the model
                BRepModel& sketches = dynamic_cast<ShapeModel*>(model)->sketches();
                Body* wireRectangleBody = BRepFixtures::wireRectangle(lowerLeft, upperRight);
                size_t index = sketches.addBody(wireRectangleBody);
                std::cerr << "added Wire Rectangle" << std::endl;      // ---LOGGING---

                // update the product hierarchy in the client
                dispatchProductActionsForBody(doc, *wireRectangleBody, index, "sketch", "shape/workplanes");

                // update the scene in the client
                dispatchGraphicsActionsForSketch(doc, *wireRectangleBody);
            });
        }

        void addWireRoundRect(Document& doc, const json& payload) {
            // This action adds a wire rounded rectangle as a sketch.
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::array({0,0,0})),
                payload.value("upperRight", json::array({1,1,0}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;
            double cornerRadius = payload.value("cornerRadius", 0.2);

            Store& store = doc.storeAt("shape");
            store.changeState([lowerLeft, upperRight, cornerRadius, &doc](Model* model) {
                // update the model
                BRepModel& sketches = dynamic_cast<ShapeModel*>(model)->sketches();
                Body* wireRectangleBody = BRepFixtures::wireRoundRect(lowerLeft, upperRight, cornerRadius);
                size_t index = sketches.addBody(wireRectangleBody);
                std::cerr << "added Wire Rounded Rectangle" << std::endl;      // ---LOGGING---

                // update the product hierarchy in the client
                dispatchProductActionsForBody(doc, *wireRectangleBody, index, "sketch", "shape/workplanes");

                // update the scene in the client
                dispatchGraphicsActionsForSketch(doc, *wireRectangleBody);
            });
        }

        void addSheetRectangle(Document& doc, const json& payload) {
            // This action adds a sheet rectangle as a profile
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::array({0,0,0})),
                payload.value("upperRight", json::array({1,1,0}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;

            Store& store = doc.storeAt("shape");
            store.changeState([lowerLeft, upperRight, &doc](Model* model) {

                // update the model
                BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                Body* sheetRectangleBody = BRepFixtures::sheetRectangle(lowerLeft, upperRight);
                size_t index = profiles.addBody(sheetRectangleBody);
                std::cerr << "added Sheet Rectangle" << std::endl;      // ---LOGGING---

                // update the product hierarchy in the client
                dispatchProductActionsForBody(doc, *sheetRectangleBody, index, "profile", "shape/profiles");

                // update the scene in the client
                dispatchGraphicsActionsForProfile(doc, *sheetRectangleBody);

            });
        }

        void addSheetRoundRect(Document& doc, const json& payload) {
            // This action adds a sheet rounded rectangle as a profile
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::array({0,0,0})),
                payload.value("upperRight", json::array({1,1,0}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;
            double cornerRadius = payload.value("cornerRadius", 0.2);

            Store& store = doc.storeAt("shape");
            store.changeState([lowerLeft, upperRight, cornerRadius, &doc](Model* model) {

                // update the model
                BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                Body* sheetRoundRectBody = BRepFixtures::sheetRoundRect(lowerLeft, upperRight, cornerRadius);
                size_t index = profiles.addBody(sheetRoundRectBody);
                std::cerr << "added Sheet Rounded Rectangle" << std::endl;      // ---LOGGING---

                // update the product hierarchy in the client
                dispatchProductActionsForBody(doc, *sheetRoundRectBody, index, "profile", "shape/profiles");

                // update the scene in the client
                dispatchGraphicsActionsForProfile(doc, *sheetRoundRectBody);
            });
        }

        void addEmptyObject(Document& doc, const json& payload) {
            // This action adds an empty FObject to the shape store.
            Store& store = doc.storeAt("shape");
            store.changeState([&doc](Model* model) {
                FRepModel& objects = dynamic_cast<ShapeModel*>(model)->objects();
                FObject* emptyObject = FRepFixtures::emptyObject();
                size_t index = objects.addObject(emptyObject);
                std::cerr << "added Empty FObject" << std::endl;      // ---LOGGING---

                // update the product hierarchy in the client
                dispatchProductActionsForObject(doc, *emptyObject, index, "object", "shape/objects");

            });
        }

        void addSphereObject(Document& doc, const json& payload) {
            // This action adds a sphere FObject to the shape store.
            double radius = payload.value("radius", 1.0);

            Store& store = doc.storeAt("shape");
            store.changeState([radius, &doc](Model* model) {

                // update the model
                FRepModel& objects = dynamic_cast<ShapeModel*>(model)->objects();
                FObject* sphereObject = FRepFixtures::sphere(radius);
                size_t index = objects.addObject(sphereObject);
                std::cerr << "added Sphere FObject" << std::endl;      // ---LOGGING---

                // update the product hierarchy in the client
                dispatchProductActionsForObject(doc, *sphereObject, index, "object", "shape/objects");

                // update the scene in the client
                dispatchGraphicsActionsForObject(doc, *sphereObject, radius * 3, radius * 3, radius * 2 * 0.99);
            });
        }

        void addBlockObject(Document& doc, const json& payload) {
            // This action adds a block FObject to the shape store.
            double width = payload.value("width", 1.0);
            double height = payload.value("height", 1.0);
            double depth = payload.value("depth", 1.0);

            Store& store = doc.storeAt("shape");
            store.changeState([width, height, depth, &doc](Model* model) {

                // update the model
                FRepModel& objects = dynamic_cast<ShapeModel*>(model)->objects();
                FObject* blockObject = FRepFixtures::block(width, height, depth);
                size_t index = objects.addObject(blockObject);
                std::cerr << "added Block FObject" << std::endl;      // ---LOGGING---

                // update the product hierarchy in the client
                dispatchProductActionsForObject(doc, *blockObject, index, "object", "shape/objects");

                // update the scene in the client
                dispatchGraphicsActionsForObject(doc, *blockObject, width * 2, height * 2, depth * 1.5);
            });
        }

        void addCylinderObject(Document& doc, const json& payload) {
            // This action adds a block FObject to the shape store.
            double radius = payload.value("radius", 1.0);
            double depth = payload.value("depth", 1.0);

            Store& store = doc.storeAt("shape");
            store.changeState([radius, depth, &doc](Model* model) {

                // update the model
                FRepModel& objects = dynamic_cast<ShapeModel*>(model)->objects();
                FObject* cylinderObject = FRepFixtures::cylinder(radius, depth);
                size_t index = objects.addObject(cylinderObject);
                std::cerr << "added Cylinder FObject" << std::endl;      // ---LOGGING---

                // update the product hierarchy in the client
                dispatchProductActionsForObject(doc, *cylinderObject, index, "object", "shape/objects");

                // update the scene in the client
                dispatchGraphicsActionsForObject(doc, *cylinderObject, radius * 2, radius * 2, depth * 1.5);
            });
        }
        
        void addInfiniteRectangle(Document& doc, const json& payload) {
            // This action adds a 2d rectangle, with infinite z-extent, as an FObject
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::array({0,0,0})),
                payload.value("upperRight", json::array({1,1,0}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;

            Store& store = doc.storeAt("shape");
            store.changeState([lowerLeft, upperRight, &doc](Model* model) {

                // update the model
                FRepModel& objects = dynamic_cast<ShapeModel*>(model)->objects();
                FObject* rectangleObject = FRepFixtures::infiniteRectangle(lowerLeft, upperRight);
                size_t index = objects.addObject(rectangleObject);
                std::cerr << "added Infinite Rectangle" << std::endl;      // ---LOGGING---

                // update the product hierarchy in the client
                dispatchProductActionsForObject(doc, *rectangleObject, index, "object", "shape/objects");

                // update the scene in the client
                double width = upperRight.x() - lowerLeft.x();
                double height = upperRight.y() - lowerLeft.y();
                dispatchGraphicsActionsForObject(doc, *rectangleObject, width * 3, height * 3);
            });
        }

        void addCappedRectangle(Document& doc, const json& payload) {
            // This action adds a capped rectangle as an FObject
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::array({0,0,0})),
                payload.value("upperRight", json::array({1, 1, 0}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;
            double depth = payload.value("depth", 1.0);

            Store& store = doc.storeAt("shape");
            store.changeState([lowerLeft, upperRight, depth, &doc](Model* model) {

                // update the model
                FRepModel& objects = dynamic_cast<ShapeModel*>(model)->objects();
                FObject* rectangleObject = FRepFixtures::cappedRectangle(lowerLeft, upperRight, depth);
                size_t index = objects.addObject(rectangleObject);
                std::cerr << "added Capped Rectangle" << std::endl;      // ---LOGGING---

                // update the product hierarchy in the client
                dispatchProductActionsForObject(doc, *rectangleObject, index, "object", "shape/objects");    

                // update the scene in the client
                double width = upperRight.x() - lowerLeft.x();
                double height = upperRight.y() - lowerLeft.y();
                dispatchGraphicsActionsForObject(doc, *rectangleObject, width * 3, height * 3, depth * 1.5);
            });
        }

        void addExtrudedRectangle(Document& doc, const json& payload) {
            // This action adds an extruded rectangle as an FObject
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::array({0,0,0})),
                payload.value("upperRight", json::array({1,1,0}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;
            double depth = payload.value("depth", 1.0);

            Store& store = doc.storeAt("shape");
            store.changeState([lowerLeft, upperRight, depth, &doc](Model* model) {

                // update the model
                FRepModel& objects = dynamic_cast<ShapeModel*>(model)->objects();
                FObject* rectangleObject = FRepFixtures::extrudedRectangle(lowerLeft, upperRight, depth);
                size_t index = objects.addObject(rectangleObject);
                std::cerr << "added Extruded Rectangle" << std::endl;      // ---LOGGING---

                // update the product hierarchy in the client
                dispatchProductActionsForObject(doc, *rectangleObject, index, "object", "shape/objects");

                // update the scene in the client
                //double width = upperRight.x() - lowerLeft.x();
                //double height = upperRight.y() - lowerLeft.y();
                //dispatchGraphicsActionsForObject(doc, *rectangleObject, width * 3, height * 3, depth * 1.5);

                // update the scene in the client
                dispatchGraphicsActionsForScene(doc);
            });
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

                    // update the product hierarchy in the client
                    dispatchProductActionsForNewFeature(doc, index);
                    // update the graphical scene in the client
                    dispatchGraphicsActionsForNewFeature(doc, index);
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

                    // update the product hierarchy in the client
                    dispatchProductActionsForNewFeature(doc, index);
                    // update the graphical scene in the client
                    dispatchGraphicsActionsForNewFeature(doc, index);
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

                    // update the product hierarchy in the client
                    dispatchProductActionsForNewFeature(doc, index);
                    // update the graphical scene in the client
                    dispatchGraphicsActionsForNewFeature(doc, index);
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

                    // update the product hierarchy in the client
                    dispatchProductActionsForNewFeature(doc, featureIndex);
                    // update the graphical scene in the client
                    dispatchGraphicsActionsForNewFeature(doc, featureIndex);
                    dispatchGraphicsActionsForNewProfile(doc, profileIndex);
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

                    // update the product hierarchy in the client
                    dispatchProductActionsForNewFeature(doc, featureIndex);
                    // update the graphical scene in the client
                    dispatchGraphicsActionsForNewFeature(doc, featureIndex);
                    dispatchGraphicsActionsForNewProfile(doc, profileIndex);
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

                    // update the product hierarchy in the client
                    dispatchProductActionsForNewFeature(doc, featureIndex);
                    // update the graphical scene in the client
                    dispatchGraphicsActionsForNewFeature(doc, featureIndex);
                    dispatchGraphicsActionsForNewProfile(doc, profileIndex);
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

                // update the product hierarchy in the client
                dispatchProductActionsForNewFeature(doc, index);
                // update the graphical scene in the client
                dispatchGraphicsActionsForNewFeature(doc, index);
            });     
        }
    }
};


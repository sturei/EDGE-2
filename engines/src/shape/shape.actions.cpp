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
                llJson.at("x").get<double>(),
                llJson.at("y").get<double>(),
                llJson.at("z").get<double>()
            );
            Vec3d upperRight = Vec3d(
                urJson.at("x").get<double>(),
                urJson.at("y").get<double>(),
                urJson.at("z").get<double>()
            );
            return std::make_pair(lowerLeft, upperRight);
        }

        void addEmptyBody(Document* doc, const json& payload) {
            // This action adds an empty body (a body with no cells) to the brep store.
            Store* store = doc->storeAt("shape");
            store->changeState([](Model* model) {
                BRepModel& sketches = dynamic_cast<ShapeModel*>(model)->sketches();
                Body* emptyBody = BRepFixtures::emptyBody();
                sketches.addBody(emptyBody);
                std::cerr << "added Empty Body" << std::endl;      // ---LOGGING---
            });
        
        }

        void addAcornBody(Document* doc, const json& payload) {
            // This action adds an acorn body (a body with a single vertex) to the brep store.
            json position = payload.value("position", json::object({{"x", 0}, {"y", 0}, {"z", 0}}));
            Vec3d acornPosition = Vec3d(
                position.at("x").get<double>(),
                position.at("y").get<double>(),
                position.at("z").get<double>()
            );

            Store* store = doc->storeAt("shape");
            store->changeState([acornPosition, doc](Model* model) {

                // update the model
                BRepModel& sketches = dynamic_cast<ShapeModel*>(model)->sketches();
                Body* acornBody = BRepFixtures::acornBody(acornPosition);
                sketches.addBody(acornBody);
                std::cerr << "added Acorn Body" << std::endl;      // ---LOGGING--- 

                // update the scene in the client
                dispatchClientActionsForAcorn(doc, *acornBody);
            });
        }

        void addWireRectangle(Document* doc, const json& payload) {
            // This action adds a wire rectangle as a sketch.
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::object({{"x", -1}, {"y", -1}, {"z", 0}})),
                payload.value("upperRight", json::object({{"x", 1 }, {"y", 1}, {"z", 0}}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;

            Store* store = doc->storeAt("shape");
            store->changeState([lowerLeft, upperRight, doc](Model* model) {

                // update the model
                BRepModel& sketches = dynamic_cast<ShapeModel*>(model)->sketches();
                Body* wireRectangleBody = BRepFixtures::wireRectangle(lowerLeft, upperRight);
                sketches.addBody(wireRectangleBody);
                std::cerr << "added Wire Rectangle" << std::endl;      // ---LOGGING---

                // update the scene in the client
                dispatchClientActionsForSketch(doc, *wireRectangleBody);
            });
        }

        void addWireRoundRect(Document* doc, const json& payload) {
            // This action adds a wire rounded rectangle as a sketch.
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::object({{"x", -1}, {"y", -1}, {"z", 0}})),
                payload.value("upperRight", json::object({{"x", 1 }, {"y", 1}, {"z", 0}}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;
            double cornerRadius = payload.value("cornerRadius", 0.2);

            Store* store = doc->storeAt("shape");
            store->changeState([lowerLeft, upperRight, cornerRadius, doc](Model* model) {
                // update the model
                BRepModel& sketches = dynamic_cast<ShapeModel*>(model)->sketches();
                Body* wireRectangleBody = BRepFixtures::wireRoundRect(lowerLeft, upperRight, cornerRadius);
                sketches.addBody(wireRectangleBody);
                std::cerr << "added Wire Rounded Rectangle" << std::endl;      // ---LOGGING---

                // update the scene in the client
                dispatchClientActionsForSketch(doc, *wireRectangleBody);
            });
        }

        void addSheetRectangle(Document* doc, const json& payload) {
            // This action adds a sheet rectangle as a profile
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::object({{"x", -1}, {"y", -1}, {"z", 0}})),
                payload.value("upperRight", json::object({{"x", 1 }, {"y", 1}, {"z", 0}}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;

            Store* store = doc->storeAt("shape");
            store->changeState([lowerLeft, upperRight, doc](Model* model) {

                // update the model
                BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                Body* sheetRectangleBody = BRepFixtures::sheetRectangle(lowerLeft, upperRight);
                profiles.addBody(sheetRectangleBody);
                std::cerr << "added Sheet Rectangle" << std::endl;      // ---LOGGING---

                // update the scene in the client
                dispatchClientActionsForProfile(doc, *sheetRectangleBody);

            });
        }

        void addSheetRoundRect(Document* doc, const json& payload) {
            // This action adds a sheet rounded rectangle as a profile
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::object({{"x", -1}, {"y", -1}, {"z", 0}})),
                payload.value("upperRight", json::object({{"x", 1 }, {"y", 1}, {"z", 0}}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;
            double cornerRadius = payload.value("cornerRadius", 0.2);

            Store* store = doc->storeAt("shape");
            store->changeState([lowerLeft, upperRight, cornerRadius, doc](Model* model) {

                // update the model
                BRepModel& profiles = dynamic_cast<ShapeModel*>(model)->profiles();
                Body* sheetRoundRectBody = BRepFixtures::sheetRoundRect(lowerLeft, upperRight, cornerRadius);
                profiles.addBody(sheetRoundRectBody);
                std::cerr << "added Sheet Rounded Rectangle" << std::endl;      // ---LOGGING---

                // update the scene in the client
                dispatchClientActionsForProfile(doc, *sheetRoundRectBody);
            });
        }

        void addInfiniteRectangle(Document* doc, const json& payload) {
            // This action adds a 2d rectangle, with infinite z-extent, as an FObject
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::object({{"x", -1}, {"y", -1}, {"z", 0}})),
                payload.value("upperRight", json::object({{"x", 1 }, {"y", 1}, {"z", 0}}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;

            Store* store = doc->storeAt("shape");
            store->changeState([doc, lowerLeft, upperRight](Model* model) {

                // update the model
                FRepModel& objects = dynamic_cast<ShapeModel*>(model)->objects();
                FObject* rectangleObject = FRepFixtures::rectangle(lowerLeft, upperRight);
                objects.addObject(rectangleObject);
                std::cerr << "added Infinite Rectangle" << std::endl;      // ---LOGGING---

                // update the scene in the client
                dispatchClientActionsForObject(doc, *rectangleObject);
            });
        }
    }
};


#include <nlohmann/json.hpp>
#include "brep/body.h"
#include "brep/brep.actions.h"
#include "brep/brep.fixtures.h"
#include "brep/brepModel.h"
#include "document/document.h"
#include "document/store.h"
#include "utils/vec3d.h"
using json = nlohmann::json;

/**
 * BRepActions provides a set of actions that enable clients to create and manipulate breps via the DocumentService.
 * These actions are intended to be registered with a Document via Document::registerActionFunction, and then invoked via actions dispatched to the Document.
 * Each action function takes a JSON payload as input.
 * Any long-lived bodies created here are owned by the Document via the Store that contains the BRepModel.
 */

namespace e2 {
    namespace BRepActions {

        void addEmptyBody(Document* doc, const json& payload) {
            // This action adds an empty body (a body with no cells) to the brep store.
            Store* store = doc->storeAt("brep");
            store->changeState([](Model* model) {
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body* emptyBody = BRepFixtures::createEmptyBody();
                brepModel->addBody(emptyBody);
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

            Store* store = doc->storeAt("brep");
            store->changeState([acornPosition](Model* model) {
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body* acornBody = BRepFixtures::createAcornBody(acornPosition);
                brepModel->addBody(acornBody);
                std::cerr << "added Acorn Body" << std::endl;      // ---LOGGING--- 
            });
        }

        void addWireRectangle(Document* doc, const json& payload) {
            // This action adds a wire rectangle to the brep store.
            json ll = payload.value("lowerLeft", json::object({{"x", -1}, {"y", -1}, {"z", 0}}));
            json ur = payload.value("upperRight", json::object({{"x", 1 }, {"y", 1}, {"z", 0}}));
            Vec3d lowerLeft = Vec3d(
                ll.at("x").get<double>(),
                ll.at("y").get<double>(),
                ll.at("z").get<double>()
            );
            Vec3d upperRight = Vec3d(
                ur.at("x").get<double>(),
                ur.at("y").get<double>(),
                ur.at("z").get<double>()
            );
            Store* store = doc->storeAt("brep");
            store->changeState([lowerLeft, upperRight](Model* model) {
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body* wireRectangleBody = BRepFixtures::createWireRectangle(lowerLeft, upperRight);
                brepModel->addBody(wireRectangleBody);
                std::cerr << "added Wire Rectangle" << std::endl;      // ---LOGGING---
            });
        }

        void addSheetRectangle(Document* doc, const json& payload) {
            // This action adds a sheet rectangle to the brep store.
            json ll = payload.value("lowerLeft", json::object({{"x", -1}, {"y", -1}, {"z", 0}}));
            json ur = payload.value("upperRight", json::object({{"x", 1 }, {"y", 1}, {"z", 0}}));
            Vec3d lowerLeft = Vec3d(
                ll.at("x").get<double>(),
                ll.at("y").get<double>(),
                0
            );
            Vec3d upperRight = Vec3d(
                ur.at("x").get<double>(),
                ur.at("y").get<double>(),
                0
            );
            Store* store = doc->storeAt("brep");
            store->changeState([lowerLeft, upperRight, doc](Model* model) {

                // update the BRepModel
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body* sheetRectangleBody = BRepFixtures::createSheetRectangle(lowerLeft, upperRight);
                brepModel->addBody(sheetRectangleBody);
                std::cerr << "added Sheet Rectangle" << std::endl;      // ---LOGGING---

                // update the Scene model in the client      
                Vec3d lowerRight(upperRight.x(), lowerLeft.y(), lowerLeft.z());
                Vec3d upperLeft(lowerLeft.x(), upperRight.y(), upperRight.z());          
                json points = json::array({
                    lowerLeft.x(), lowerLeft.y(),
                    lowerRight.x(), lowerRight.y(),
                    upperRight.x(), upperRight.y(),
                    upperLeft.x(), upperLeft.y(),
                    lowerLeft.x(), lowerLeft.y()
                });
                json clientPayload = json::object({{"points", points}});
                doc->dispatchClientAction({"addGShape", clientPayload});

            });
           }
    }
};


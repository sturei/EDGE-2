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
            });
        }

        void addWireRectangle(Document* doc, const json& payload) {
            // This action adds a wire rectangle to the brep store.
            Store* store = doc->storeAt("brep");
            store->changeState([](Model* model) {
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body* wireRectangleBody = BRepFixtures::createWireRectangle();
                brepModel->addBody(wireRectangleBody);
            });
           }
        void addSheetRectangle(Document* doc, const json& payload) {
            // This action adds a sheet rectangle to the brep store.
            Store* store = doc->storeAt("brep");
            store->changeState([](Model* model) {
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body* sheetRectangleBody = BRepFixtures::createSheetRectangle();
                brepModel->addBody(sheetRectangleBody);
            });
           }
    }
};


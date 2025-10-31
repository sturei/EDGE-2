#include <nlohmann/json.hpp>
#include "brep/body.h"
#include "brep/brep.actions.h"
#include "brep/brep.fixtures.h"
#include "brep/brepModel.h"
#include "document/document.h"
#include "document/store.h"
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
    }
};


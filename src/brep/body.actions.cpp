#include <nlohmann/json.hpp>
#include "brep/body.h"
#include "brep/body.actions.h"
#include "brep/brepModel.h"
#include "document/document.h"
#include "document/store.h"
using json = nlohmann::json;

/**
 * BodyActions provides a set of actions that enable clients to create and manipulate brep bodies via the DocumentService.
 * These actions are intended to be registered with a Document via Document::registerActionFunction, and then invoked via actions dispatched to the Document.
 * Each action function takes a JSON payload as input.
 * Any long-lived bodies created here are owned by the Document via the Store that contains the BRepModel.
 */

namespace e2 {
    namespace BodyActions {
        void addEmptyBody(Document* doc, const json& payload) {
            // This action adds an empty body to the BRepModel in the "brep" store of the document.
            Store* store = doc->storeAt("brep");
            store->changeState([](Model* model) {
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body emptyBody;
                brepModel->addBody(emptyBody);
            });
        }
    }
};


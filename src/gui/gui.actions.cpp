#include "document/document.h"
#include "document/store.h"
#include "gui/gui.actions.h"
#include "gui/guiModel.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

/**
 * GuiActions provides a set of actions that enable clients to create and manipulate the display list, and potentially other aspects of the GUI.
 */

namespace e2 {
    namespace GuiActions {
        void ping(Document* doc, const json& payload) {
            // This action just writes "pong" to stderr. Useful for testing connectivity.
            std::cerr << "pong" << std::endl;
        }
        void addGPoint(Document* doc, const json& payload) {
            std::cerr << "TODO: addGPoint" << std::endl;
        }
        void addGLine(Document* doc, const json& payload) {
            std::cerr << "TODO: addGLine" << std::endl;
        }
        void addGPlane(Document* doc, const json& payload) {
            std::cerr << "TODO: addGPlane" << std::endl;
        }
        void addGSphere(Document* doc, const json& payload) {
            std::cerr << "TODO: addGSphere" << std::endl;
        }
        void addGBlock(Document* doc, const json& payload) {
            Store* store = doc->storeAt("gui");
            store->changeState([payload](Model* model) {
                double width = payload.value("width", 2.0);
                double height = payload.value("height", 2.0);
                double length = payload.value("length", 2.0);
                GuiModel* guiModel = dynamic_cast<GuiModel*>(model);
                GBlock* block = new GBlock(width, height, length);
                guiModel->addGItem(block);
            });
            std::cerr << "added GBlock" << std::endl;      // ---IGNORE---
        }
        void addGGroup(Document* doc, const json& payload) {
            std::cerr << "TODO: addGGroup" << std::endl;
        }
    }
};


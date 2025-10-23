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
            Store* store = doc->storeAt("gui");
            store->changeState([payload](Model* model) {
                double size = payload.value("size", 1.0);    // size of the point
                GuiModel* guiModel = dynamic_cast<GuiModel*>(model);
                GPoint* point = new GPoint(size);
                guiModel->addGItem(point);
            });
            std::cerr << "added GPoint" << std::endl;      // ---IGNORE---
        }

        void addGLine(Document* doc, const json& payload) {
            Store* store = doc->storeAt("gui");
            store->changeState([payload](Model* model) {
                double length = payload.value("length", 1.0);    // length of the line
                GuiModel* guiModel = dynamic_cast<GuiModel*>(model);
                GLine* line = new GLine(length);
                guiModel->addGItem(line);
            });
            std::cerr << "added GLine" << std::endl;      // ---IGNORE---
        }
        
        void addGPlane(Document* doc, const json& payload) {
            Store* store = doc->storeAt("gui");
            store->changeState([payload](Model* model) {
                double width = payload.value("width", 1.0);      // width of the plane
                double height = payload.value("height", 1.0);    // height of the plane
                GuiModel* guiModel = dynamic_cast<GuiModel*>(model);
                GPlane* plane = new GPlane(width, height);
                guiModel->addGItem(plane);
            });
            std::cerr << "added GPlane" << std::endl;      // ---IGNORE---
        }

        void addGSphere(Document* doc, const json& payload) {
            Store* store = doc->storeAt("gui");
            store->changeState([payload](Model* model) {
                double radius = payload.value("radius", 1.0);    // radius of the sphere
                GuiModel* guiModel = dynamic_cast<GuiModel*>(model);
                GSphere* sphere = new GSphere(radius);
                guiModel->addGItem(sphere);
            });
            std::cerr << "added GSphere" << std::endl;      // ---IGNORE---
        }

        void addGBlock(Document* doc, const json& payload) {
            Store* store = doc->storeAt("gui");
            store->changeState([payload](Model* model) {
                double width = payload.value("width", 2.0);      // length in x direction
                double height = payload.value("height", 2.0);    // length in y direction
                double depth = payload.value("depth", 2.0);      // length in z direction
                GuiModel* guiModel = dynamic_cast<GuiModel*>(model);
                GBlock* block = new GBlock(width, height, depth);
                guiModel->addGItem(block);
            });
            std::cerr << "added GBlock" << std::endl;      // ---IGNORE---
        }
    }
};


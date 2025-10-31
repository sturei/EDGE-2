#include "document/document.h"
#include "document/store.h"
#include "grep/grep.actions.h"
#include "grep/grepModel.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

/**
 * GrepActions provides a set of actions that enable clients to create and manipulate the display list, and potentially other aspects of the GUI.
 */

namespace e2 {
    namespace GrepActions {
        void ping(Document* doc, const json& payload) {
            // This action just writes "pong" to stderr. Useful for testing connectivity.
            std::cerr << "pong" << std::endl;
        }
        void addGPoint(Document* doc, const json& payload) {
            Store* store = doc->storeAt("grep");
            store->changeState([payload](Model* model) {
                double size = payload.value("size", 1.0);    // size of the point
                GrepModel* grepModel = dynamic_cast<GrepModel*>(model);
                GPoint* point = new GPoint(size);
                grepModel->addGItem(point);
            });
            std::cerr << "added GPoint" << std::endl;      // ---IGNORE---
        }

        void addGLine(Document* doc, const json& payload) {
            Store* store = doc->storeAt("grep");
            store->changeState([payload](Model* model) {
                double length = payload.value("length", 1.0);    // length of the line
                GrepModel* grepModel = dynamic_cast<GrepModel*>(model);
                GLine* line = new GLine(length);
                grepModel->addGItem(line);
            });
            std::cerr << "added GLine" << std::endl;      // ---IGNORE---
        }
        
        void addGPlane(Document* doc, const json& payload) {
            Store* store = doc->storeAt("grep");
            store->changeState([payload](Model* model) {
                double width = payload.value("width", 1.0);      // width of the plane
                double height = payload.value("height", 1.0);    // height of the plane
                GrepModel* grepModel = dynamic_cast<GrepModel*>(model);
                GPlane* plane = new GPlane(width, height);
                grepModel->addGItem(plane);
            });
            std::cerr << "added GPlane" << std::endl;      // ---IGNORE---
        }

        void addGSphere(Document* doc, const json& payload) {
            Store* store = doc->storeAt("grep");
            store->changeState([payload](Model* model) {
                double radius = payload.value("radius", 1.0);    // radius of the sphere
                GrepModel* grepModel = dynamic_cast<GrepModel*>(model);
                GSphere* sphere = new GSphere(radius);
                grepModel->addGItem(sphere);
            });
            std::cerr << "added GSphere" << std::endl;      // ---IGNORE---
        }

        void addGBlock(Document* doc, const json& payload) {
            Store* store = doc->storeAt("grep");
            store->changeState([payload](Model* model) {
                double width = payload.value("width", 2.0);      // length in x direction
                double height = payload.value("height", 2.0);    // length in y direction
                double depth = payload.value("depth", 2.0);      // length in z direction
                GrepModel* grepModel = dynamic_cast<GrepModel*>(model);
                GBlock* block = new GBlock(width, height, depth);
                grepModel->addGItem(block);
            });
            std::cerr << "added GBlock" << std::endl;      // ---IGNORE---
        }
    }
};


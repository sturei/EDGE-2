#include "document/document.h"
#include "scene/scene.actions.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

/**
 * SceneActions is just a placeholder for now.
 * Eventually, the "scene" will be another grepModel, that combines the various models (brepModel, grepModel, etc) into a single drawlist for rendering.
 */

namespace e2 {
    namespace SceneActions {
        void getScene(Document* doc, const json& payload) {
            //TODO: get the scene, convert it to JSON and stream it back through the Document to the client.
            std::cerr << "getScene action invoked" << std::endl;
        }
    }
};


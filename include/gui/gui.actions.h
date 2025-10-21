#pragma once
#include "document/document.h"
#include "document/store.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

/**
 * GuiActions provides a set of actions that enable clients to create and manipulate the display list, and potentially other aspects of the GUI.
 */

namespace e2 {
    namespace GuiActions {
        void ping(Document* doc, const json& payload) {
            // This action just writes "pong" to stderr. Can be useful for testing connectivity.
            std::cerr << "pong" << std::endl;
        }
        Document::ActionDef pingDef = {"ping", ping};
    }
};


#include "document/document.h"
#include "document/document.actions.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

/**
 * DocumentActions is just a placeholder for now.
 */

namespace e2 {
    namespace DocumentActions {
        void ping(Document* doc, const json& payload) {
            // This action just writes "pong" to stderr. Can be useful sometimes for testing connectivity.
            std::cerr << "pong" << std::endl;
        }
    }
};


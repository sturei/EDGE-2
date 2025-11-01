#pragma once

#include <nlohmann/json.hpp>
#include "document/document.h"

namespace e2 {
    namespace DocumentActions {
        void ping(Document* doc, const nlohmann::json& payload);
        inline static ActionDef pingDef = {"ping", ping};
    }
};


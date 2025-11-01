#pragma once

#include <nlohmann/json.hpp>
#include "document/document.h"

namespace e2 {
    namespace SceneActions {
        void getScene(Document* doc, const nlohmann::json& payload);
        inline static ActionDef getSceneDef = {"getScene", getScene};
    }
};


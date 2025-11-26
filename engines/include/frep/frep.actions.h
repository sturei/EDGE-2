#pragma once

#include <nlohmann/json.hpp>
#include "document/document.h"

namespace e2 {
    namespace FRepActions {
        void addRectangle(Document* doc, const nlohmann::json& payload);

        inline static ActionDef addRectangleDef = {"Objects::addInfiniteRectangle", addRectangle};
    }
};


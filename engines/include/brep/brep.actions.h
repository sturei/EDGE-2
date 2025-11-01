#pragma once

#include <nlohmann/json.hpp>
#include "document/document.h"

namespace e2 {
    namespace BRepActions {
        void addEmptyBody(Document* doc, const nlohmann::json& payload);

        inline static ActionDef addEmptyBodyDef = {"addEmptyBody", addEmptyBody};           
    }
};


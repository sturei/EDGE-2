#pragma once

#include <nlohmann/json.hpp>
#include "document/document.h"

namespace e2 {
    namespace BodyActions {
        void addEmptyBody(Document* doc, const nlohmann::json& payload);

        inline static Document::ActionDef addEmptyBodyDef = {"addEmptyBody", addEmptyBody};           
    }
};


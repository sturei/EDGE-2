#pragma once

#include <nlohmann/json.hpp>
#include "document/document.h"

namespace e2 {
    namespace ShapeActions {
        void addPrimitive2D(Document& doc, const nlohmann::json& payload);
        void addPrimitive(Document& doc, const nlohmann::json& payload);
        void addExtrusion(Document& doc, const nlohmann::json& payload);

        inline static std::array<ActionDef, 3> allShapeActions = {{
            {"Profiles::addPrimitive", addPrimitive2D},
            {"Features::addPrimitive", addPrimitive},
            {"Features::addExtrusion", addExtrusion}
        }};
    }
};


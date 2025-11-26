#pragma once

#include <nlohmann/json.hpp>
#include "document/document.h"

namespace e2 {
    namespace ShapeActions {
        void addEmptyBody(Document* doc, const nlohmann::json& payload);
        void addAcornBody(Document* doc, const nlohmann::json& payload);
        void addWireRectangle(Document* doc, const nlohmann::json& payload);
        void addWireRoundRect(Document* doc, const nlohmann::json& payload);
        void addSheetRectangle(Document* doc, const nlohmann::json& payload);
        void addSheetRoundRect(Document* doc, const nlohmann::json& payload);
        void addInfiniteRectangle(Document* doc, const nlohmann::json& payload);

        inline static std::array<ActionDef, 7> allShapeActions = {{
            {"Sketches::addEmptyBody", addEmptyBody},
            {"Sketches::addAcornBody", addAcornBody},
            {"Sketches::addRectangle", addWireRectangle},
            {"Sketches::addRoundRect", addWireRoundRect},
            {"Profiles::addRectangle", addSheetRectangle},
            {"Profiles::addRoundRect", addSheetRoundRect},
            {"Objects::addInfiniteRectangle", addInfiniteRectangle}
        }};
    }
};


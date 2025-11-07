#pragma once

#include <nlohmann/json.hpp>
#include "document/document.h"

namespace e2 {
    namespace BRepActions {
        void addEmptyBody(Document* doc, const nlohmann::json& payload);
        void addAcornBody(Document* doc, const nlohmann::json& payload);
        void addWireRectangle(Document* doc, const nlohmann::json& payload);
        void addSheetRectangle(Document* doc, const nlohmann::json& payload);
        void addWireRoundRect(Document* doc, const nlohmann::json& payload);
        void addSheetRoundRect(Document* doc, const nlohmann::json& payload);

        inline static ActionDef addEmptyBodyDef = {"addEmptyBody", addEmptyBody};
        inline static ActionDef addAcornBodyDef = {"addAcornBody", addAcornBody};
        inline static ActionDef addWireRectangleDef = {"addWireRectangle", addWireRectangle};
        inline static ActionDef addSheetRectangleDef = {"addSheetRectangle", addSheetRectangle};
        inline static ActionDef addWireRoundRectDef = {"addWireRoundRect", addWireRoundRect};
        inline static ActionDef addSheetRoundRectDef = {"addSheetRoundRect", addSheetRoundRect};
    }
};


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

        inline static ActionDef addEmptyBodyDef = {"BRep::addEmptyBody", addEmptyBody};
        inline static ActionDef addAcornBodyDef = {"BRep::addAcornBody", addAcornBody};
        inline static ActionDef addWireRectangleDef = {"BRep::addWireRectangle", addWireRectangle};
        inline static ActionDef addSheetRectangleDef = {"BRep::addSheetRectangle", addSheetRectangle};
        inline static ActionDef addWireRoundRectDef = {"BRep::addWireRoundRect", addWireRoundRect};
        inline static ActionDef addSheetRoundRectDef = {"BRep::addSheetRoundRect", addSheetRoundRect};
    }
};


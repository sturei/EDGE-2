#pragma once

#include <nlohmann/json.hpp>
#include "document/document.h"

namespace e2 {
    namespace BRepActions {
        void addEmptyBody(Document* doc, const nlohmann::json& payload);
        void addAcornBody(Document* doc, const nlohmann::json& payload);
        void addWireRectangle(Document* doc, const nlohmann::json& payload);
        void addWireRoundRect(Document* doc, const nlohmann::json& payload);
        void addSheetRectangle(Document* doc, const nlohmann::json& payload);
        void addSheetRoundRect(Document* doc, const nlohmann::json& payload);

        inline static ActionDef addEmptyBodyDef = {"Sketches::addEmptyBody", addEmptyBody};
        inline static ActionDef addAcornBodyDef = {"Sketches::addAcornBody", addAcornBody};
        inline static ActionDef addWireRectangleDef = {"Sketches::addRectangle", addWireRectangle};
        inline static ActionDef addWireRoundRectDef = {"Sketches::addRoundRect", addWireRoundRect};
        inline static ActionDef addSheetRectangleDef = {"Profiles::addRectangle", addSheetRectangle};
        inline static ActionDef addSheetRoundRectDef = {"Profiles::addRoundRect", addSheetRoundRect};
    }
};


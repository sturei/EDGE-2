#pragma once

#include <nlohmann/json.hpp>
#include "document/document.h"

namespace e2 {
    namespace GuiActions {
        void ping(Document* doc, const nlohmann::json& payload);
        void addGPoint(Document* doc, const nlohmann::json& payload);
        void addGLine(Document* doc, const nlohmann::json& payload);
        void addGPlane(Document* doc, const nlohmann::json& payload);
        void addGSphere(Document* doc, const nlohmann::json& payload);
        void addGBlock(Document* doc, const nlohmann::json& payload);
        void addGGroup(Document* doc, const nlohmann::json& payload);

        inline static Document::ActionDef pingDef = {"ping", ping};
        inline static Document::ActionDef addGPointDef = {"addGPoint", addGPoint};
        inline static Document::ActionDef addGLineDef = {"addGLine", addGLine};
        inline static Document::ActionDef addGPlaneDef = {"addGPlane", addGPlane};
        inline static Document::ActionDef addGSphereDef = {"addGSphere", addGSphere};
        inline static Document::ActionDef addGBlockDef = {"addGBlock", addGBlock};
        inline static Document::ActionDef addGGroupDef = {"addGGroup", addGGroup};
    }
};


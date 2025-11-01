#pragma once

#include <nlohmann/json.hpp>
#include "document/document.h"

namespace e2 {
    namespace GRepActions {
        void ping(Document* doc, const nlohmann::json& payload);
        void addGPoint(Document* doc, const nlohmann::json& payload);
        void addGLine(Document* doc, const nlohmann::json& payload);
        void addGPlane(Document* doc, const nlohmann::json& payload);
        void addGSphere(Document* doc, const nlohmann::json& payload);
        void addGBlock(Document* doc, const nlohmann::json& payload);
        //void addGMesh(Document* doc, const nlohmann::json& payload);
        //void addGGroup(Document* doc, const nlohmann::json& payload);
        // et cetera - names, transformations, appearances, transform_item, remove_item, etc.

        inline static ActionDef pingDef = {"ping", ping};
        inline static ActionDef addGPointDef = {"addGPoint", addGPoint};
        inline static ActionDef addGLineDef = {"addGLine", addGLine};
        inline static ActionDef addGPlaneDef = {"addGPlane", addGPlane};
        inline static ActionDef addGSphereDef = {"addGSphere", addGSphere};
        inline static ActionDef addGBlockDef = {"addGBlock", addGBlock};
    }
};


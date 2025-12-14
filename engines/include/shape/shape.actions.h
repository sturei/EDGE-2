#pragma once

#include <nlohmann/json.hpp>
#include "document/document.h"

namespace e2 {
    namespace ShapeActions {
        void addEmptyBody(Document& doc, const nlohmann::json& payload);
        void addAcornBody(Document& doc, const nlohmann::json& payload);
        void addWireRectangle(Document& doc, const nlohmann::json& payload);
        void addWireRoundRect(Document& doc, const nlohmann::json& payload);
        void addSheetRectangle(Document& doc, const nlohmann::json& payload);
        void addSheetRoundRect(Document& doc, const nlohmann::json& payload);
        void addEmptyObject(Document& doc, const nlohmann::json& payload);
        void addSphereObject(Document& doc, const nlohmann::json& payload);
        void addBlockObject(Document& doc, const nlohmann::json& payload);
        void addCylinderObject(Document& doc, const nlohmann::json& payload);
        void addInfiniteRectangle(Document& doc, const nlohmann::json& payload);
        void addCappedRectangle(Document& doc, const nlohmann::json& payload);
        void addExtrudedRectangle(Document& doc, const nlohmann::json& payload);
        void addPrimitive(Document& doc, const nlohmann::json& payload);
        void addPrimitive2D(Document& doc, const nlohmann::json& payload);
        void addExtrusion(Document& doc, const nlohmann::json& payload);

        inline static std::array<ActionDef, 16> allShapeActions = {{
            {"Sketches::addEmptyBody", addEmptyBody},
            {"Sketches::addAcornBody", addAcornBody},
            {"Sketches::addRectangle", addWireRectangle},
            {"Sketches::addRoundRect", addWireRoundRect},
            {"Profiles::addRectangle", addSheetRectangle},
            {"Profiles::addRoundRect", addSheetRoundRect},
            {"Objects::addEmptyObject", addEmptyObject},
            {"Objects::addSphereObject", addSphereObject},
            {"Objects::addBlockObject", addBlockObject},
            {"Objects::addCylinderObject", addCylinderObject},
            {"Objects::addInfiniteRectangle", addInfiniteRectangle},
            {"Objects::addCappedRectangle", addCappedRectangle},
            {"Objects::addExtrudedRectangle", addExtrudedRectangle},
            {"Features::addPrimitive", addPrimitive},
            {"Features::addPrimitive2D", addPrimitive2D},          // eventually the name of this action will be changed to Profiles::addPrimitive, replacing the outdated Profiles::addRectangle and Profiles::addRoundRect actions
            {"Features::addExtrusion", addExtrusion}
        }};
    }
};


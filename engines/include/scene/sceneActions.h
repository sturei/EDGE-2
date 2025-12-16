#pragma once
#include "brep/body.h"
#include "frep.deprecated/fobject.h"
#include "document/document.h"

namespace e2 {
    void dispatchGraphicsActionsForAcorn(Document& doc, const Body& acornBody);
    void dispatchGraphicsActionsForSketch(Document& doc, const Body& sketchBody);
    void dispatchGraphicsActionsForProfile(Document& doc, const Body& profileBody);
    void dispatchGraphicsActionsForObject(Document& doc, const FObject& fobject, double width = 20.0, double height = 20.0, double depth = 20.0);
    void dispatchGraphicsActionsForScene(Document& doc);

    void dispatchProductActionsForBody(Document& doc, const Body& body, size_t bodyIndex, const std::string& bodyType, const std::string& parentPathName);
    void dispatchProductActionsForObject(Document& doc, const FObject& fobject, size_t objectIndex, const std::string& objectType, const std::string& parentPathName);

    void dispatchProductActionsForNewFeature(Document& doc, size_t featureIndex);
    void dispatchGraphicsActionsForNewFeature(Document& doc, size_t featureIndex);
    void dispatchGraphicsActionsForNewProfile(Document& doc, size_t profileIndex);

};

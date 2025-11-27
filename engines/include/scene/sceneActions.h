#pragma once
#include "brep/body.h"
#include "frep/fobject.h"
#include "document/document.h"

namespace e2 {
    void dispatchClientActionsForAcorn(Document& doc, const Body& acornBody);
    void dispatchClientActionsForSketch(Document& doc, const Body& sketchBody);
    void dispatchClientActionsForProfile(Document& doc, const Body& profileBody);
    void dispatchClientActionsForObject(Document& doc, const FObject& fobject, double width = 20.0, double height = 20.0, double depth = 20.0);
};

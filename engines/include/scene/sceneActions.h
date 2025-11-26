#pragma once
#include "brep/body.h"
#include "frep/fobject.h"
#include "document/document.h"

namespace e2 {
    void dispatchClientActionsForAcorn(Document& doc, const Body& acornBody);
    void dispatchClientActionsForSketch(Document& doc, const Body& sketchBody);
    void dispatchClientActionsForProfile(Document& doc, const Body& profileBody);
    void dispatchClientActionsForObject(Document& doc, const FObject& fobject);
};

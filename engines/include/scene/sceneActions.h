#pragma once
#include "document/document.h"

namespace e2 {
    void dispatchProductActionsForNewFeature(Document& doc, size_t featureIndex);
    void dispatchGraphicsActionsForNewFeature(Document& doc, size_t featureIndex);
    void dispatchGraphicsActionsForNewProfile(Document& doc, size_t profileIndex);
};

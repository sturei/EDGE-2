#pragma once
#include <nlohmann/json.hpp>
#include "brep/body.h"
#include "frep/fobject.h"
#include "document/document.h"

namespace e2 {
    // This function returns the JSON payload for the addGProfile action.
    void getPayloadForAddGProfile(const Body& profileBody, nlohmann::json& payload);
    void getClientActionsForAddFObject(const FObject& fobject, std::vector<ActionSpec>& actions);
};

#pragma once
#include <nlohmann/json.hpp>
#include "brep/body.h"

namespace e2 {
    // This function returns the JSON payload for the addGProfile action.
    void getPayloadForAddGProfile(const Body& profileBody, nlohmann::json& payload);
};

#include <nlohmann/json.hpp>
#include "brep/body.h"
#include "utils/vec3d.h"
#include "brep/navigate.h"
#include "brep/btessellate.h"

using json = nlohmann::json;

/**
 * Scene Actions are dispatched by action routines to keep the graphics scene in sync with the models.
 * The graphics scene is in fact managed in the client application. 
 * Hence these actions are so-called "client actions".
 * This module provides utilities to construct the payloads for those actions.
 */

namespace e2 {
    // This function returns the JSON payload for the addGProfile action.
    void getPayloadForAddGProfile(const Body& profileBody, json& payload) {
        // The payload is a collection of "paths", representing the outline of the given body.
        CellIndex faceIndex = getFacesOfBody(profileBody)[0];
        auto edges = getEdgesOfFace(faceIndex, profileBody);
        std::vector<json> paths;
        for (const auto& edgePair : edges) {
            CellIndex edgeIndex = edgePair.first;
            auto tessellatedPointsPtr = tessellate(profileBody, edgeIndex);
            std::vector<json> path;
            for (const auto& point : *tessellatedPointsPtr) {
                    path.push_back(json::array({point.x(), point.y()}));
            }
            if (edgePair.second == -1) {
                std::reverse(path.begin(), path.end());
            }
            paths.push_back(path);
            delete tessellatedPointsPtr;
        }

        payload = json::object({{"paths", paths}});
    }
};


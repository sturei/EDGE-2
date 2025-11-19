#include <nlohmann/json.hpp>
#include "brep/brep.fixtures.h"
#include "frep/frep.fixtures.h"
#include "shape/shapeModel.h"
#include "document/document.h"
#include "document/store.h"
#include "utils/vec3d.h"

using json = nlohmann::json;

/**
 * FRepActions provides a set of actions that enable clients to create and manipulate freps via the DocumentService.
 */

namespace e2 {
    namespace FRepActions {

        std::pair<Vec3d, Vec3d> parseLowerUpperJson(const json& llJson, const json& urJson) {
            Vec3d lowerLeft = Vec3d(
                llJson.at("x").get<double>(),
                llJson.at("y").get<double>(),
                llJson.at("z").get<double>()
            );
            Vec3d upperRight = Vec3d(
                urJson.at("x").get<double>(),
                urJson.at("y").get<double>(),
                urJson.at("z").get<double>()
            );
            return std::make_pair(lowerLeft, upperRight);
        }

        void addRectangle(Document* doc, const json& payload) {
            // This action adds a 2d rectangle to the frep store.
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::object({{"x", -1}, {"y", -1}, {"z", 0}})),
                payload.value("upperRight", json::object({{"x", 1 }, {"y", 1}, {"z", 0}}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;

            Store* store = doc->storeAt("shape");
            store->changeState([lowerLeft, upperRight](Model* model) {
                FRepModel* frepModel = dynamic_cast<ShapeModel*>(model)->frepModel();
                FObject* rectangleObject = FRepFixtures::rectangle(lowerLeft, upperRight);
                frepModel->addObject(rectangleObject);
                std::cerr << "added Rectangle" << std::endl;      // ---LOGGING---
            });
        }

    }
};


#include <nlohmann/json.hpp>
#include "brep/body.h"
#include "brep/brep.actions.h"
#include "brep/brep.fixtures.h"
#include "brep/brepModel.h"
#include "brep/navigate.h"
#include "brep/tessellate.h"
#include "document/document.h"
#include "document/store.h"
#include "utils/vec3d.h"
using json = nlohmann::json;

/**
 * BRepActions provides a set of actions that enable clients to create and manipulate breps via the DocumentService.
 * These actions are intended to be registered with a Document via Document::registerActionFunction, and then invoked via actions dispatched to the Document.
 * Each action function takes a JSON payload as input.
 * Any long-lived bodies created here are owned by the Document via the Store that contains the BRepModel.
 */

namespace e2 {
    namespace BRepActions {

        void addEmptyBody(Document* doc, const json& payload) {
            // This action adds an empty body (a body with no cells) to the brep store.
            Store* store = doc->storeAt("brep");
            store->changeState([](Model* model) {
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body* emptyBody = BRepFixtures::createEmptyBody();
                brepModel->addBody(emptyBody);
                std::cerr << "added Empty Body" << std::endl;      // ---LOGGING---
            });
        
        }

        void addAcornBody(Document* doc, const json& payload) {
            // This action adds an acorn body (a body with a single vertex) to the brep store.
            json position = payload.value("position", json::object({{"x", 0}, {"y", 0}, {"z", 0}}));
            Vec3d acornPosition = Vec3d(
                position.at("x").get<double>(),
                position.at("y").get<double>(),
                position.at("z").get<double>()
            );

            Store* store = doc->storeAt("brep");
            store->changeState([acornPosition](Model* model) {
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body* acornBody = BRepFixtures::createAcornBody(acornPosition);
                brepModel->addBody(acornBody);
                std::cerr << "added Acorn Body" << std::endl;      // ---LOGGING--- 
            });
        }

        void getPayloadForGProfile(const Body& profile, json& clientPayload) {
            // This function generates the JSON payload for a GProfile representing the outline of the given sheet rectangle body.
            CellIndex faceIndex = getFacesOfBody(profile)[0];
            auto edges = getEdgesOfFace(faceIndex, profile);
            std::vector<json> paths;
            for (const auto& edgePair : edges) {
                CellIndex edgeIndex = edgePair.first;
                auto tessellatedPointsPtr = tessellate(profile, edgeIndex);
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

            clientPayload = json::object({{"paths", paths}});
        }

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

        void addWireRectangle(Document* doc, const json& payload) {
            // This action adds a wire rectangle to the brep store.
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::object({{"x", -1}, {"y", -1}, {"z", 0}})),
                payload.value("upperRight", json::object({{"x", 1 }, {"y", 1}, {"z", 0}}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;

            Store* store = doc->storeAt("brep");
            store->changeState([lowerLeft, upperRight](Model* model) {
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body* wireRectangleBody = BRepFixtures::createWireRectangle(lowerLeft, upperRight);
                brepModel->addBody(wireRectangleBody);
                std::cerr << "added Wire Rectangle" << std::endl;      // ---LOGGING---
            });
        }

        void addSheetRectangle(Document* doc, const json& payload) {
            // This action adds a sheet rectangle to the brep store.
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::object({{"x", -1}, {"y", -1}, {"z", 0}})),
                payload.value("upperRight", json::object({{"x", 1 }, {"y", 1}, {"z", 0}}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;

            Store* store = doc->storeAt("brep");
            store->changeState([lowerLeft, upperRight, doc](Model* model) {

                // update the BRepModel
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body* sheetRectangleBody = BRepFixtures::createSheetRectangle(lowerLeft, upperRight);
                brepModel->addBody(sheetRectangleBody);
                std::cerr << "added Sheet Rectangle" << std::endl;      // ---LOGGING---

                // update the scene in the client
                json clientPayload;
                getPayloadForGProfile(*sheetRectangleBody, clientPayload);
                doc->dispatchClientAction({"addGProfile", clientPayload});

            });
        }

        void addWireRoundRect(Document* doc, const json& payload) {
            // This action adds a wire rounded rectangle to the brep store.
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::object({{"x", -1}, {"y", -1}, {"z", 0}})),
                payload.value("upperRight", json::object({{"x", 1 }, {"y", 1}, {"z", 0}}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;
            double cornerRadius = payload.value("cornerRadius", 0.2);

            Store* store = doc->storeAt("brep");
            store->changeState([lowerLeft, upperRight, cornerRadius](Model* model) {
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body* wireRectangleBody = BRepFixtures::createWireRoundRect(lowerLeft, upperRight, cornerRadius);
                brepModel->addBody(wireRectangleBody);
                std::cerr << "added Wire Rounded Rectangle" << std::endl;      // ---LOGGING---
            });
        }

        void addSheetRoundRect(Document* doc, const json& payload) {
            // This action adds a sheet rounded rectangle to the brep store.
            std::pair<Vec3d, Vec3d> bounds = parseLowerUpperJson(
                payload.value("lowerLeft", json::object({{"x", -1}, {"y", -1}, {"z", 0}})),
                payload.value("upperRight", json::object({{"x", 1 }, {"y", 1}, {"z", 0}}))
            );
            Vec3d lowerLeft = bounds.first;
            Vec3d upperRight = bounds.second;
            double cornerRadius = payload.value("cornerRadius", 0.2);

            Store* store = doc->storeAt("brep");
            store->changeState([lowerLeft, upperRight, cornerRadius, doc](Model* model) {

                // update the BRepModel
                BRepModel* brepModel = dynamic_cast<BRepModel*>(model);
                Body* sheetRoundRectBody = BRepFixtures::createSheetRoundRect(lowerLeft, upperRight, cornerRadius);
                brepModel->addBody(sheetRoundRectBody);
                std::cerr << "added Sheet Rounded Rectangle" << std::endl;      // ---LOGGING---

                // update the scene in the client
                json clientPayload;
                getPayloadForGProfile(*sheetRoundRectBody, clientPayload);
                doc->dispatchClientAction({"addGProfile", clientPayload});

            });
        }



    }
};


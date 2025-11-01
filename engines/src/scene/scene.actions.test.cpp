#include <gtest/gtest.h>
#include "scene/scene.actions.h"
#include "document/document.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace e2;

TEST(SceneActionsTest, GetSceneWritesToStderr) {
    testing::internal::CaptureStderr();
    json payload = json::object();  
    Document doc;

    SceneActions::getScene(&doc, payload);

    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "getScene action invoked\n");
}

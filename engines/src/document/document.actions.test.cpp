#include <gtest/gtest.h>
#include "document/document.actions.h"
#include "document/document.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace e2;

TEST(DocumentActionsTest, PingWritesToStderr) {
    testing::internal::CaptureStderr();
    json payload = json::object();  
    Document doc;

    DocumentActions::ping(doc, payload);

    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "pong\n");
}

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "document/documentService.h"

using namespace e2;

// consider moving ZooModelFixture and addAnimal to a shared test utils file if needed elsewhere
struct ZooModelFixture : public Model {
    std::vector<std::string> animals;
    void print(std::ostream& os) const override {
        os << "ZooModel with " << animals.size() << " animals." << std::endl;
        for (const auto& animal : animals) {
            os << "  Animal: " << animal << std::endl;
        }
    }
};

namespace DocumentServiceTestActions {
    void addAnimal(Document* doc, const json& payload) {
        // This is a simple action function that adds an animal to the zoo model.

        //unpack the payload
        const std::string& species = payload.at("species");

        // add the animal to the model via the state change callback on the store.
        Store* store = doc->storeAt("zoo");
        store->changeState([species](Model* model) {
            auto* zooModel = dynamic_cast<ZooModelFixture*>(model);
            zooModel->animals.push_back(species);
        });
    }
    ActionDef addAnimalDef = {"addAnimal", addAnimal};
}

class DocumentServiceTest : public ::testing::Test {
 protected:
  void SetUp() override {
        // Implementation notes:
        // As a unit test, probably this should have used a Mock Document instead.
        Model* zooModel = new ZooModelFixture();
        Store* zooStore = new Store(zooModel);            // store takes ownership of the model
        document = new Document({{"zoo", zooStore}});     // document takes ownership of the store
        document->registerActionFunction(DocumentServiceTestActions::addAnimalDef);
    }

    void TearDown() override {
        delete document;
    }
    Document *document;
};

TEST_F(DocumentServiceTest, RunOnce) {

    // create a single-line JSON text with type, payload pair
    std::string actionText = std::string(R"(
    {
        "type": "addAnimal",
        "payload": { "species": "Giraffe" }
    }
    )");
    actionText.erase(remove(actionText.begin(), actionText.end(), '\n'), actionText.end());
    
    // Simulate input and output streams using stringstreams.
    std::istringstream input(actionText);
    std::ostringstream output;
    DocumentService::runOnce(document, input, output);
    std::string outputStr = output.str();
    EXPECT_TRUE(outputStr.find("\"status\":\"OK\"") != std::string::npos);
    EXPECT_TRUE(outputStr.find("\"reason\":\"\"") != std::string::npos);
};

TEST_F(DocumentServiceTest, RunOnceEmptyLine) {
    std::istringstream input("");
    std::ostringstream output;
    DocumentService::runOnce(document, input, output);
    std::string outputStr = output.str();
    EXPECT_TRUE(outputStr.find("\"status\":\"ERROR\"") != std::string::npos);
    EXPECT_TRUE(outputStr.find("Invalid stream or EOF") != std::string::npos);
}

TEST_F(DocumentServiceTest, RunOnceEmptyLineString) {
    std::istringstream input("\n");
    std::ostringstream output;
    DocumentService::runOnce(document, input, output);
    std::string outputStr = output.str();
    EXPECT_TRUE(outputStr.find("\"status\":\"OK\"") != std::string::npos);
    EXPECT_TRUE(outputStr.find("Empty line") != std::string::npos);
}

TEST_F(DocumentServiceTest, RunOnceInvalidJSON) {
    std::string invalidJson = "{ invalid json }";
    std::istringstream input(invalidJson);
    std::ostringstream output;
    DocumentService::runOnce(document, input, output);
    std::string outputStr = output.str();
    EXPECT_TRUE(outputStr.find("\"status\":\"ERROR\"") != std::string::npos);
    EXPECT_TRUE(outputStr.find("Invalid JSON") != std::string::npos);
}

TEST_F(DocumentServiceTest, RunOnceMissingType) {
    std::string missingType = R"({"payload": {"species": "Lion"}})";
    std::istringstream input(missingType);
    std::ostringstream output;
    DocumentService::runOnce(document, input, output);
    std::string outputStr = output.str();
    EXPECT_TRUE(outputStr.find("\"status\":\"ERROR\"") != std::string::npos);
    EXPECT_TRUE(outputStr.find("Invalid JSON") != std::string::npos);
}

TEST_F(DocumentServiceTest, RunOnceMissingPayload) {
    std::string missingPayload = R"({"type": "addAnimal"})";
    std::istringstream input(missingPayload);
    std::ostringstream output;
    DocumentService::runOnce(document, input, output);
    std::string outputStr = output.str();
    EXPECT_TRUE(outputStr.find("\"status\":\"ERROR\"") != std::string::npos);
    EXPECT_TRUE(outputStr.find("Invalid JSON") != std::string::npos);
}

TEST_F(DocumentServiceTest, RunOnceUnknownAction) {
    std::string unknownAction = R"({"type": "unknownAction", "payload": {}})";
    std::istringstream input(unknownAction);
    std::ostringstream output;
    DocumentService::runOnce(document, input, output);
    std::string outputStr = output.str();
    EXPECT_TRUE(outputStr.find("\"status\":\"ERROR\"") != std::string::npos);
    EXPECT_TRUE(outputStr.find("Unknown action type") != std::string::npos);
}

TEST_F(DocumentServiceTest, RunOnceInvalidPayload) {
    std::string invalidPayload = R"({"type": "addAnimal", "payload": {"wrongField": "value"}})";
    std::istringstream input(invalidPayload);
    std::ostringstream output;
    DocumentService::runOnce(document, input, output);
    std::string outputStr = output.str();
    EXPECT_TRUE(outputStr.find("\"status\":\"ERROR\"") != std::string::npos);
    EXPECT_TRUE(outputStr.find("Invalid payload") != std::string::npos);
}



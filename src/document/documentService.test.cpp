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
}

class DocumentServiceTest : public ::testing::Test {
 protected:
  void SetUp() override {
        // Implementation notes:
        // As a unit test, probably this should have used a Mock Document instead.
        Model* zooModel = new ZooModelFixture();
        Store* zooStore = new Store(zooModel);            // store takes ownership of the model
        document = new Document({{"zoo", zooStore}});     // document takes ownership of the store
        document->registerActionFunction("addAnimal", DocumentServiceTestActions::addAnimal);
    }

    void TearDown() override {
        delete document;
    }
    Document *document;
};

TEST_F(DocumentServiceTest, Run) {

    // a single-line JSON text with type, payload pair
    std::string actionText = std::string(R"(
    {
        "type": "addAnimal",
        "payload": { "species": "Giraffe" }
    }
    )");
    actionText.erase(remove(actionText.begin(), actionText.end(), '\n'), actionText.end());
    
    // Simulate input and output streams using stringstreams
    std::istringstream input(actionText);
    std::ostringstream output;
    std::ostringstream error;
    DocumentService::run(document, input, output, error);
    std::string outputStr = output.str();
    std::string errorStr = error.str(); 

    // std::cout << "Output stream:\n" << outputStr << std::endl; // --- IGNORE ---
    // std::cout << "Error stream:\n" << errorStr << std::endl; // --- IGNORE ---

    EXPECT_TRUE(outputStr.find("Giraffe") != std::string::npos);          
    EXPECT_EQ(errorStr, "");
};

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "document/documentService.h"

using namespace e2;

struct ZooModelFixture : public Model{
    std::vector<std::string> animals;
};

namespace DocumentServiceTestActions {
    void addAnimal(Document* doc, void* payload) {
        // This is a simple action function that adds an animal to the zoo model.
        const std::string& animal = *static_cast<const std::string*>(payload);
        Store* store = doc->storeAt("zoo");
        store->changeState([animal](Model* model) {
            auto* zooModel = dynamic_cast<ZooModelFixture*>(model);
            zooModel->animals.push_back(animal);
        });
    }
}

class DocumentServiceTest : public ::testing::Test {
 protected:
  void SetUp() override {
Model* zooModel = new ZooModelFixture();
        Store* zooStore = new Store(zooModel);            // store takes ownership of the model
        Document* document = new Document({{"zoo", zooStore}});     // document takes ownership of the store
        documentService = new DocumentService(document);     // documentService takes ownership of the document
        document->registerActionFunction("addAnimal", DocumentServiceTestActions::addAnimal);
    }

    void TearDown() override {
      delete documentService;
    }
    DocumentService *documentService;
};

TEST_F(DocumentServiceTest, DefaultConstructor) {
    DocumentService docService(nullptr);
    // Just a basic test to check that the DocumentService can be constructed.
    EXPECT_TRUE(true);
};

TEST_F(DocumentServiceTest, Run) {

    // TODO: find a nice way to make json literals.

    std::string actions = "{\"type\":\"addAnimal\", \"payload\" : \"Giraffe\"}";

    // Simulate input and output streams using stringstreams
    std::istringstream input(actions);
    std::ostringstream output;
    std::ostringstream error;
    documentService->run(output, input, error);
    std::string outputStr = output.str();
    std::string errorStr = error.str(); 
    EXPECT_TRUE(outputStr.find("Giraffe") != std::string::npos);          
    EXPECT_EQ(errorStr, "");
};

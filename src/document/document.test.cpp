#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "document/document.h"

using namespace e2;

struct ZooModelFixture : public Model{
    std::vector<std::string> animals;
};

class DocumentTest : public ::testing::Test {
 protected:
  void SetUp() override {
      Model* zooModel = new ZooModelFixture();
      Store* zooStore = new Store(zooModel);            // store takes ownership of the model
      document = new Document({{"zoo", zooStore}});     // document takes ownership of the store
    }

    void TearDown() override {
      delete document;
    }
    Document *document;
};

namespace DocumentTestActions {
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

TEST_F(DocumentTest, DefaultConstructor) {
    Document doc;
    // Just a basic test to check that the Document can be constructed.
    EXPECT_TRUE(true);
};

TEST_F(DocumentTest, AddAnimalAction) {
    // Directly invoke the addAnimal action function to add an animal to the zoo model.
    json payload =
    {
        {"species", "Elephant"}
    };
    DocumentTestActions::addAnimal(document, payload);
    Store* store = document->storeAt("zoo");
    auto* zooModel = dynamic_cast<const ZooModelFixture*>(store->model());
    EXPECT_EQ(zooModel->animals.size(), 1);
    EXPECT_EQ(zooModel->animals[0], "Elephant");
};

TEST_F(DocumentTest, RegisterAndDispatchAction) {
    // Invoke the addAnimal action function via the Document's dispatchAction method.

    // Register the addAnimal action function
    document->registerActionFunction("addAnimal", DocumentTestActions::addAnimal);

    // Construct the action (type, payload) pair, and dispatch it
    ActionSpec action = {
        "addAnimal", {{"species", "Giraffe"}}
    };

    document->dispatchAction(action);

    // Verify that the animal was added to the model
    Store* store = document->storeAt("zoo");
    auto* zooModel = dynamic_cast<const ZooModelFixture*>(store->model());
    EXPECT_EQ(zooModel->animals.size(), 1);
    EXPECT_EQ(zooModel->animals[0], "Giraffe");
};

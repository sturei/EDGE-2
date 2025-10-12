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

TEST_F(DocumentTest, DefaultConstructor) {
    Document doc;
    // Just a basic test to check that the Document can be constructed.
    EXPECT_TRUE(true);
};

TEST_F(DocumentTest, AddAnimalAction) {
    std::string animal = "Elephant";
    DocumentTestActions::addAnimal(document, &animal);
    Store* store = document->storeAt("zoo");
    auto* zooModel = dynamic_cast<const ZooModelFixture*>(store->model());
    EXPECT_EQ(zooModel->animals.size(), 1);
    EXPECT_EQ(zooModel->animals[0], "Elephant");
};

TEST_F(DocumentTest, AddMultipleAnimals) {
    std::vector<std::string> animals = {"Lion", "Tiger", "Bear"};
    for (const auto& animal : animals) {
        DocumentTestActions::addAnimal(document, (void*)&animal);
    }
    Store* store = document->storeAt("zoo");
    auto* zooModel = dynamic_cast<const ZooModelFixture*>(store->model());
    EXPECT_EQ(zooModel->animals.size(), animals.size());
    for (size_t i = 0; i < animals.size(); ++i) {
        EXPECT_EQ(zooModel->animals[i], animals[i]);
    }
};

TEST_F(DocumentTest, RegisterAndDispatchAction) {
    // Register the addAnimal action function
    document->registerActionFunction("addAnimal", DocumentTestActions::addAnimal);

    // Dispatch an action to add an animal
    std::string animal = "Giraffe";
    ActionSpec action;
    action.type = "addAnimal";
    action.payload = &animal;
    document->dispatchAction(action);

    // Verify that the animal was added
    Store* store = document->storeAt("zoo");
    auto* zooModel = dynamic_cast<const ZooModelFixture*>(store->model());
    EXPECT_EQ(zooModel->animals.size(), 1);
    EXPECT_EQ(zooModel->animals[0], "Giraffe");
};

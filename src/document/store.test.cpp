#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "document/store.h"
#include "document/model.h"

using namespace e2;

struct ZooModelFixture : public Model{
    std::vector<std::string> animals;
};

class StoreTest : public ::testing::Test {
 protected:
  void SetUp() override {
      model = new ZooModelFixture();
      store = new Store(model);
    }

    void TearDown() override {
      delete store;  // also deletes model
    }
    ZooModelFixture* model;
    Store* store;
};

TEST_F(StoreTest, ChangeState) {
    bool callbackCalled = false;
    std::string animal = "Lion";
    store->changeState([&callbackCalled, animal](Model* model) {
        callbackCalled = true;
        auto* zooModel = dynamic_cast<ZooModelFixture*>(model);
        zooModel->animals.push_back(animal);
    });
    EXPECT_TRUE(callbackCalled);
    auto* zooModel = dynamic_cast<const ZooModelFixture*>(store->model());
    EXPECT_EQ(zooModel->animals.size(), 1);
    EXPECT_EQ(zooModel->animals[0], "Lion");
};

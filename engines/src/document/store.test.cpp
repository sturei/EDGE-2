#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "document/store.h"
#include "document/model.h"

using namespace e2;

struct ZooModelFixture : public Model{
    std::vector<std::string> animals;
    void print(std::ostream& os) const override {
        os << "ZooModel with " << animals.size() << " animals." << std::endl;
        for (const auto& animal : animals) {
            os << "  Animal: " << animal << std::endl;
        }
    }
};

class StoreTest : public ::testing::Test {
 protected:
  void SetUp() override {
      Model* zooModel = new ZooModelFixture();
      store = new Store(zooModel);
    }

    void TearDown() override {
      delete store;
    }
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

TEST_F(StoreTest, StreamOutputOperator) {
    std::ostringstream oss;
    oss << *store;
    std::string storeStr = oss.str();

    // std::cout << "Store output:\n" << storeStr << std::endl; // --- IGNORE ---

    // Check that the output string contains some expected substrings
    EXPECT_NE(storeStr.find("ZooModel with 0 animals."), std::string::npos);

    // Add an animal and check output again
    std::string animal = "Lion";
    store->changeState([animal](Model* model) {
        auto* zooModel = dynamic_cast<ZooModelFixture*>(model);
        zooModel->animals.push_back(animal);
    });

    oss.str(""); // clear the stream
    oss << *store;
    storeStr = oss.str();
    EXPECT_NE(storeStr.find("ZooModel with 1 animals."), std::string::npos);
}

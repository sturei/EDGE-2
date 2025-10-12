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
      Store* zooStore = new Store(zooModel);
      document = new Document({{"zoo", zooStore}});
    }

    void TearDown() override {
      delete document;
    }
    Document *document;
};

TEST_F(DocumentTest, DefaultConstructor) {
    Document doc;
    // Just a basic test to check that the Document can be constructed.
    // More tests will be added later.
    EXPECT_TRUE(true);
};
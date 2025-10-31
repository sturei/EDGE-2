#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "grep/grep.actions.h"
#include "document/document.h"
#include "document/store.h"
#include "grep/grepModel.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace e2;

class MockDocument : public Document {
public:    
    MOCK_METHOD(Store*, storeAt, (const std::string&), (const, override));
};

class MockStore : public Store {
public:
    MOCK_METHOD(void, changeState, (std::function<void(Model*)>), (override));
};

class MockGrepModel : public GrepModel {
public:
    MOCK_METHOD(size_t, addGItem, (GItem*), (override));
};

class GrepActionsTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockDoc = std::make_unique<MockDocument>();
        mockStore = std::make_unique<MockStore>();
        mockModel = std::make_unique<MockGrepModel>();

        ON_CALL(*mockDoc, storeAt("grep"))
            .WillByDefault(::testing::Return(mockStore.get()));

        ON_CALL(*mockStore, changeState(::testing::_))
            .WillByDefault([this](std::function<void(Model*)> callback) {
                callback(mockModel.get());
            });
        
        ON_CALL(*mockModel, addGItem(::testing::_))
            .WillByDefault([](GItem* item) {
                return 0;
            });

    }

    std::unique_ptr<MockDocument> mockDoc;
    std::unique_ptr<MockStore> mockStore;
    std::unique_ptr<MockGrepModel> mockModel;
};


TEST_F(GrepActionsTest, PingWritesToStderr) {
    testing::internal::CaptureStderr();
    json payload = json::object();
    
    GrepActions::ping(mockDoc.get(), payload);
    
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "pong\n");
}


TEST_F(GrepActionsTest, AddGPointWithDefaultSize) {
    json payload = json::object();

    EXPECT_CALL(*mockDoc, storeAt("grep"))
        .WillOnce(::testing::Return(mockStore.get()));
    
    EXPECT_CALL(*mockStore, changeState(::testing::_))
        .WillOnce([this](std::function<void(Model*)> callback) {
            callback(mockModel.get());
        });
    
    EXPECT_CALL(*mockModel, addGItem(::testing::_))
        .Times(1);
    
    testing::internal::CaptureStderr();
    GrepActions::addGPoint(mockDoc.get(), payload);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "added GPoint\n");
}


TEST_F(GrepActionsTest, AddGPointWithCustomSize) {
    json payload = json::object({{"size", 2.5}});
    
    EXPECT_CALL(*mockDoc, storeAt("grep"))
        .WillOnce(::testing::Return(mockStore.get()));
    
    EXPECT_CALL(*mockStore, changeState(::testing::_))
        .WillOnce([this](std::function<void(Model*)> callback) {
            callback(mockModel.get());
        });
    
    // In typescipt it was easy to actaually check that the size was passed correctly.
    // Here we just check that addGItem was called. 
    EXPECT_CALL(*mockModel, addGItem(::testing::_))
        .Times(1);
    
    testing::internal::CaptureStderr();
    GrepActions::addGPoint(mockDoc.get(), payload);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "added GPoint\n");
}

TEST_F(GrepActionsTest, AddGLineWithDefaultLength) {
    json payload = json::object();
    
    EXPECT_CALL(*mockDoc, storeAt("grep"))
        .WillOnce(::testing::Return(mockStore.get()));
    
    EXPECT_CALL(*mockStore, changeState(::testing::_))
        .WillOnce([this](std::function<void(Model*)> callback) {
            callback(mockModel.get());
        });
    
    EXPECT_CALL(*mockModel, addGItem(::testing::_))
        .Times(1);
    
    testing::internal::CaptureStderr();
    GrepActions::addGLine(mockDoc.get(), payload);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "added GLine\n");
}


TEST_F(GrepActionsTest, AddGLineWithCustomLength) {
    json payload = json::object({{"length", 3.7}});
    
    EXPECT_CALL(*mockDoc, storeAt("grep"))
        .WillOnce(::testing::Return(mockStore.get()));
    
    EXPECT_CALL(*mockStore, changeState(::testing::_))
        .WillOnce([this](std::function<void(Model*)> callback) {
            callback(mockModel.get());
        });
    
    EXPECT_CALL(*mockModel, addGItem(::testing::_))
        .Times(1);
    
    testing::internal::CaptureStderr();
    GrepActions::addGLine(mockDoc.get(), payload);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "added GLine\n");
}


TEST_F(GrepActionsTest, AddGPlaneWithDefaultDimensions) {
    json payload = json::object();
    
    EXPECT_CALL(*mockDoc, storeAt("grep"))
        .WillOnce(::testing::Return(mockStore.get()));
    
    EXPECT_CALL(*mockStore, changeState(::testing::_))
        .WillOnce([this](std::function<void(Model*)> callback) {
            callback(mockModel.get());
        });
    
    EXPECT_CALL(*mockModel, addGItem(::testing::_))
        .Times(1);
    
    testing::internal::CaptureStderr();
    GrepActions::addGPlane(mockDoc.get(), payload);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "added GPlane\n");
}

TEST_F(GrepActionsTest, AddGPlaneWithCustomDimensions) {
    json payload = json::object({{"width", 4.5}, {"height", 6.2}});
    
    EXPECT_CALL(*mockDoc, storeAt("grep"))
        .WillOnce(::testing::Return(mockStore.get()));
    
    EXPECT_CALL(*mockStore, changeState(::testing::_))
        .WillOnce([this](std::function<void(Model*)> callback) {
            callback(mockModel.get());
        });
    
    EXPECT_CALL(*mockModel, addGItem(::testing::_))
        .Times(1);
    
    testing::internal::CaptureStderr();
    GrepActions::addGPlane(mockDoc.get(), payload);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "added GPlane\n");
}

TEST_F(GrepActionsTest, AddGSphereWithDefaultRadius) {
    json payload = json::object();
    
    EXPECT_CALL(*mockDoc, storeAt("grep"))
        .WillOnce(::testing::Return(mockStore.get()));
    
    EXPECT_CALL(*mockStore, changeState(::testing::_))
        .WillOnce([this](std::function<void(Model*)> callback) {
            callback(mockModel.get());
        });
    
    EXPECT_CALL(*mockModel, addGItem(::testing::_))
        .Times(1);
    
    testing::internal::CaptureStderr();
    GrepActions::addGSphere(mockDoc.get(), payload);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "added GSphere\n");
}

TEST_F(GrepActionsTest, AddGSphereWithCustomRadius) {
    json payload = json::object({{"radius", 2.8}});
    
    EXPECT_CALL(*mockDoc, storeAt("grep"))
        .WillOnce(::testing::Return(mockStore.get()));
    
    EXPECT_CALL(*mockStore, changeState(::testing::_))
        .WillOnce([this](std::function<void(Model*)> callback) {
            callback(mockModel.get());
        });
    
    EXPECT_CALL(*mockModel, addGItem(::testing::_))
        .Times(1);
    
    testing::internal::CaptureStderr();
    GrepActions::addGSphere(mockDoc.get(), payload);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "added GSphere\n");
}

TEST_F(GrepActionsTest, AddGBlockWithDefaultDimensions) {
    json payload = json::object();
    
    EXPECT_CALL(*mockDoc, storeAt("grep"))
        .WillOnce(::testing::Return(mockStore.get()));
    
    EXPECT_CALL(*mockStore, changeState(::testing::_))
        .WillOnce([this](std::function<void(Model*)> callback) {
            callback(mockModel.get());
        });
    
    EXPECT_CALL(*mockModel, addGItem(::testing::_))
        .Times(1);
    
    testing::internal::CaptureStderr();
    GrepActions::addGBlock(mockDoc.get(), payload);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "added GBlock\n");
}

TEST_F(GrepActionsTest, AddGBlockWithCustomDimensions) {
    json payload = json::object({{"width", 3.0}, {"height", 4.0}, {"depth", 5.0}});
    
    EXPECT_CALL(*mockDoc, storeAt("grep"))
        .WillOnce(::testing::Return(mockStore.get()));
    
    EXPECT_CALL(*mockStore, changeState(::testing::_))
        .WillOnce([this](std::function<void(Model*)> callback) {
            callback(mockModel.get());
        });
    
    EXPECT_CALL(*mockModel, addGItem(::testing::_))
        .Times(1);
    
    testing::internal::CaptureStderr();
    GrepActions::addGBlock(mockDoc.get(), payload);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "added GBlock\n");
}

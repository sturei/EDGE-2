#include <gtest/gtest.h>
#include "frep/fobject.h"


namespace e2 {

class FObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        fobject = std::make_unique<FObject>();
    }

    void TearDown() override {
        fobject.reset();
    }

    std::unique_ptr<FObject> fobject;
};

class MockEvaluator : public FEvaluator {
public:
    bool evaluate(const Vec3d& position, const std::vector<double>& inputs, double& output) const override {
        output = 42.0; // arbitrary value
        return true;
    }
    void print(std::ostream& os) const override {
        os << "MockEvaluator";
    }   
};

class MockEvaluator2 : public FEvaluator {
public:
    bool evaluate(const Vec3d& position, const std::vector<double>& inputs, double& output) const override {
        output = 1.0;   // different arbitrary value
        return true;
    }
    void print(std::ostream& os) const override {
        os << "MockEvaluator2";
    }   
};

TEST_F(FObjectTest, InitialStateIsEmpty) {
    EXPECT_EQ(fobject->numFNodes(), 0);
    EXPECT_EQ(fobject->numFArgs(), 0);
}

TEST_F(FObjectTest, AddFNodeReturnsValidIndex) {
    FNode node(FNodeType::MAX);
    FNodeIndex index = fobject->addFNode(node);
    EXPECT_EQ(index, 0);
    EXPECT_EQ(fobject->numFNodes(), 1);
}

TEST_F(FObjectTest, AddMultipleFNodes) {
    FNode node1(FNodeType::MAX);
    FNode node2(FNodeType::MIN);
    
    FNodeIndex index1 = fobject->addFNode(node1);
    FNodeIndex index2 = fobject->addFNode(node2);
    
    EXPECT_EQ(index1, 0);
    EXPECT_EQ(index2, 1);
    EXPECT_EQ(fobject->numFNodes(), 2);
}

TEST_F(FObjectTest, AddFArgReturnsValidIndex) {
    FNode node1(FNodeType::MAX);
    FNode node2(FNodeType::MIN);
    
    FNodeIndex nodeIndex1 = fobject->addFNode(node1);
    FNodeIndex nodeIndex2 = fobject->addFNode(node2);
    
    FArg arg(nodeIndex1, nodeIndex2);
    FArgIndex argIndex = fobject->addFArg(arg);
    
    EXPECT_EQ(argIndex, 0);
    EXPECT_EQ(fobject->numFArgs(), 1);
}

TEST_F(FObjectTest, RetrieveFNodeByIndex) {
    FNode node(FNodeType::MAX);
    FNodeIndex index = fobject->addFNode(node);
    
    const FNode& retrievedNode = fobject->fnode(index);
    EXPECT_EQ(retrievedNode.type(), FNodeType::MAX);
}

TEST_F(FObjectTest, RetrieveFArgByIndex) {
    FNode node1(FNodeType::MAX);
    FNode node2(FNodeType::MIN);
    
    FNodeIndex nodeIndex1 = fobject->addFNode(node1);
    FNodeIndex nodeIndex2 = fobject->addFNode(node2);
    
    FArg arg(nodeIndex1, nodeIndex2);
    FArgIndex argIndex = fobject->addFArg(arg);
    
    const FArg& retrievedArg = fobject->farg(argIndex);
    EXPECT_EQ(retrievedArg.outputFNode(), nodeIndex1);
    EXPECT_EQ(retrievedArg.inputFNode(), nodeIndex2);
}

TEST_F(FObjectTest, AttachAndFindEvaluator) {
    FNode node(FNodeType::EVALUATION);
    FNodeIndex nodeIndex = fobject->addFNode(node);
    
    MockEvaluator* evaluator = new MockEvaluator();
    fobject->attachEvaluator(nodeIndex, evaluator);
    
    FEvaluator* foundEvaluator = nullptr;
    bool found = fobject->findEvaluator(nodeIndex, foundEvaluator);
    
    EXPECT_TRUE(found);
    EXPECT_EQ(foundEvaluator, evaluator);
}

TEST_F(FObjectTest, FindNonExistentEvaluator) {
    FNode node(FNodeType::MAX);
    FNodeIndex nodeIndex = fobject->addFNode(node);
    
    FEvaluator* foundEvaluator = nullptr;
    bool found = fobject->findEvaluator(nodeIndex, foundEvaluator);
    
    EXPECT_FALSE(found);
    EXPECT_EQ(foundEvaluator, nullptr);
}

TEST_F(FObjectTest, GraphUpdateTriggered) {
    FNode node1(FNodeType::MAX);
    FNode node2(FNodeType::MIN);
    
    // Adding nodes should trigger graph update
    fobject->addFNode(node1);
    fobject->addFNode(node2);
    
    // Adding args should also trigger graph update
    FArg arg(0, 1);
    fobject->addFArg(arg);
    
    // Force graph update and verify it completes without error
    fobject->updateGraph();
    EXPECT_EQ(fobject->numFNodes(), 2);
    EXPECT_EQ(fobject->numFArgs(), 1);
}

TEST_F(FObjectTest, StreamOperatorOutput) {
    FNode node(FNodeType::MAX);
    fobject->addFNode(node);
    
    std::ostringstream oss;
    oss << *fobject;
    
    std::string output = oss.str();
    EXPECT_TRUE(output.find("FObject with 1 FNodes and 0 FArgs") != std::string::npos);
}

TEST_F(FObjectTest, FNodeStreamOperator) {
    FNode node(FNodeType::MAX);
    
    std::ostringstream oss;
    oss << node;
    
    std::string output = oss.str();
    EXPECT_TRUE(output.find("FNode(type=") != std::string::npos);
}

TEST_F(FObjectTest, FArgStreamOperator) {
    FArg arg(0, 1);
    
    std::ostringstream oss;
    oss << arg;
    
    std::string output = oss.str();
    EXPECT_TRUE(output.find("FArg(output=0, input=1)") != std::string::npos);
}

} // namespace e2
#include <gtest/gtest.h>
#include "frep/fobject.h"
#include "frep/functions.h"

using namespace e2;

class FObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        fobject = new FObject();
    }

    void TearDown() override {
        delete fobject;
    }

    FObject* fobject;
};

class MockFunction : public Function {
public:
    bool evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const override {
        valueOut = 42.0;
        return true;
    }
    void print(std::ostream& os) const override {
        os << "MockFunction";
    }
};

TEST_F(FObjectTest, InitialStateIsEmpty) {
    EXPECT_EQ(fobject->numFNodes(), 0);
    EXPECT_EQ(fobject->numFArgs(), 0);
}

TEST_F(FObjectTest, AddFunctionReturnsValidIndex) {
    Function* function = new MockFunction();
    FunctionIndex index = fobject->addFunction(function);
    EXPECT_EQ(index, 0);
    EXPECT_EQ(fobject->numFunctions(), 1);
}

TEST_F(FObjectTest, AddFNodeReturnsValidIndex) {
    FNode node(0); // Using 0 as a dummy function index
    FNodeIndex index = fobject->addFNode(node);
    EXPECT_EQ(index, 0);
    EXPECT_EQ(fobject->numFNodes(), 1);
}

TEST_F(FObjectTest, AddMultipleFNodes) {
    FNode node1(0); // Using 0 as a dummy function index
    FNode node2(1); // Using 1 as a dummy function index

    FNodeIndex index1 = fobject->addFNode(node1);
    FNodeIndex index2 = fobject->addFNode(node2);
    
    EXPECT_EQ(index1, 0);
    EXPECT_EQ(index2, 1);
    EXPECT_EQ(fobject->numFNodes(), 2);
}

TEST_F(FObjectTest, AddFArgReturnsValidIndex) {
    FNode node1(0); // Using 0 as a dummy function index
    FNode node2(1); // Using 1 as a dummy function index

    FNodeIndex nodeIndex1 = fobject->addFNode(node1);
    FNodeIndex nodeIndex2 = fobject->addFNode(node2);
    
    FArg arg(nodeIndex1, nodeIndex2);
    FArgIndex argIndex = fobject->addFArg(arg);
    
    EXPECT_EQ(argIndex, 0);
    EXPECT_EQ(fobject->numFArgs(), 1);
}

TEST_F(FObjectTest, RetrieveFNodeByIndex) {
    FNode node(0); // Using 0 as a dummy function index
    FNodeIndex index = fobject->addFNode(node);
    
    const FNode& retrievedNode = fobject->fnode(index);
    EXPECT_EQ(retrievedNode.functionIndex(), 0);
}

TEST_F(FObjectTest, RetrieveFArgByIndex) {
    FNode node1(0);
    FNode node2(1);
    
    FNodeIndex nodeIndex1 = fobject->addFNode(node1);
    FNodeIndex nodeIndex2 = fobject->addFNode(node2);
    
    FArg arg(nodeIndex1, nodeIndex2);
    FArgIndex argIndex = fobject->addFArg(arg);
    
    const FArg& retrievedArg = fobject->farg(argIndex);
    EXPECT_EQ(retrievedArg.outputFNode(), nodeIndex1);
    EXPECT_EQ(retrievedArg.inputFNode(), nodeIndex2);
}

TEST_F(FObjectTest, GraphUpdateTriggered) {
    FNode node1(0);
    FNode node2(1);

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
    FNode node(0);
    fobject->addFNode(node);
    
    std::ostringstream oss;
    oss << *fobject;
    
    std::string output = oss.str();
    EXPECT_TRUE(output.find("FObject with 1 FNodes and 0 FArgs") != std::string::npos);
}

TEST_F(FObjectTest, FNodeStreamOperator) {
    FNode node(0);
    
    std::ostringstream oss;
    oss << node;
    
    std::string output = oss.str();
    EXPECT_TRUE(output.find("FNode(functionIndex=") != std::string::npos);
}

TEST_F(FObjectTest, FArgStreamOperator) {
    FArg arg(0, 1);
    
    std::ostringstream oss;
    oss << arg;
    
    std::string output = oss.str();
    EXPECT_TRUE(output.find("FArg(output=0, input=1)") != std::string::npos);
}

class FMaxObjectTest : public ::testing::Test {
protected:
    Vec3d _position{0,0,0};         // dummy position for convenience
    FObject maxObject{
        {   // functions
            new FMax(),
            new FConstant(42.0),
            new FConstant(1.0)
        },
        {   // nodes
            FNode(0),
            FNode(1),
            FNode(2)
        },
        {   // args
            FArg(1, 0),
            FArg(2, 0)
        },
        0  // root node
    };
};

TEST_F(FMaxObjectTest, FObjectEvaluatesToMax) {
    double output;
    bool result = maxObject.evaluate(_position, output );
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, 42.0);
}

    class FNavigateTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // Setup test FObject with known structure
            const std::vector<FNode> fnodes = {
                FNode(0), // Dummy function indices
                FNode(1), // 
                FNode(2)  //
            };
            const std::vector<FArg> fargs = {
                FArg(1, 0),
                FArg(2, 0)
            };
            fobject = new FObject({}, fnodes, fargs, 0); // root is node 0
        }

        void TearDown() override {
            // Cleanup if necessary
            delete fobject; 
        }
        
        FObject* fobject;
    };

    TEST_F(FNavigateTest, ArityReturnsCorrectInDegree) {
        // Test arity function returns correct number of input connections
        FNodeIndex nodeIndex = 0;
        size_t expectedArity = 2;
        
        EXPECT_EQ(arity(*fobject, nodeIndex), expectedArity);
    }

    TEST_F(FNavigateTest, ArityHandlesLeafNode) {
        // Test arity for leaf node (no inputs)
        FNodeIndex leafNodeIndex = 1;
        
        EXPECT_EQ(arity(*fobject, leafNodeIndex), 0);
    }

    TEST_F(FNavigateTest, GetInputFNodesReturnsCorrectNodes) {
        // Test getInputFNodes returns correct input node indices
        FNodeIndex nodeIndex = 0;
        std::vector<FNodeIndex> inputs = getInputFNodes(*fobject, nodeIndex);

        EXPECT_FALSE(inputs.empty());
        EXPECT_EQ(inputs.size(), arity(*fobject, nodeIndex));
    }

    TEST_F(FNavigateTest, GetInputFNodesHandlesLeafNode) {
        // Test getInputFNodes for leaf node returns empty vector
        FNodeIndex leafNodeIndex = 1;
        std::vector<FNodeIndex> inputs = getInputFNodes(*fobject, leafNodeIndex);
        
        EXPECT_TRUE(inputs.empty());
    }

    TEST_F(FNavigateTest, NavigateTest_ArityHandlesChange) {
        // Test that arity triggers graph update when needed
        FNodeIndex nodeIndex = 0;

        fobject->addFNode(FNode(3)); // Modify object to require graph update
        fobject->addFArg(FArg(3, 0));

        // Check the updated arity
        size_t result = arity(*fobject, nodeIndex);
        
        EXPECT_EQ(result, 3);
    }

    TEST_F(FNavigateTest, GetInputFNodesHandlesChanges) {
        // Test that getInputFNodes triggers graph update when needed
        FNodeIndex nodeIndex = 0;
        
        // Modify object to require graph update
        fobject->addFNode(FNode(3)); 
        fobject->addFArg(FArg(3, 0));

        // Check the updated inputs
        std::vector<FNodeIndex> inputs = getInputFNodes(*fobject, nodeIndex);

        EXPECT_EQ(inputs.size(), 3);
    }


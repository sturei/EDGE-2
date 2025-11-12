#include <gtest/gtest.h>
#include "frep/fnavigate.h"
#include "frep/fobject.h"

namespace e2 {

    class FNavigateTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // Setup test FObject with known structure
            const std::vector<FNode> fnodes = {
                FNode(0), // Dummy evaluator indices
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

} // namespace e2
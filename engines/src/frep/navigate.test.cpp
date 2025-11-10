#include <gtest/gtest.h>
#include "frep/navigate.h"


namespace e2 {

    class MockEvaluator : public FEvaluator {
public:
    bool evaluateSDF(const FObject& fobject, FNodeIndex _nodeIndex, const Vec3d& _position, double& output) const override {
        output = 42.0; // arbitrary value
        return true;
    }
    void print(std::ostream& os) const override {
        os << "MockEvaluator";
    }   
};

class MockEvaluator2 : public FEvaluator {
public:
    bool evaluateSDF(const FObject& fobject, FNodeIndex _nodeIndex, const Vec3d& _position, double& output) const override {
        output = 1.0;   // different arbitrary value
        return true;
    }
    void print(std::ostream& os) const override {
        os << "MockEvaluator2";
    }   
};

class FObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        fMinObject = std::make_unique<FObject>();
        fMaxOfNegationObject = std::make_unique<FObject>();

        // Setup fMaxObject to compute the MAX of two evaluated nodes
        std::vector<FNode> nodes = {
            FNode(FNodeType::MAX),              // 0
            FNode(FNodeType::EVALUATION),       // 1
            FNode(FNodeType::EVALUATION)        // 2
        };
        std::vector<FArg> args = {
            FArg(1, 0),
            FArg(2, 0)
        };
        FNodeIndex root = 0;
        fMaxObject = std::make_unique<FObject>(nodes, args, root);


        fMaxObject = std::make_unique<FObject>();
        fMaxObject->addFNode(FNode(FNodeType::MAX));              // 0
        fMaxObject->addFNode(FNode(FNodeType::EVALUATION));       // 1
        fMaxObject->addFNode(FNode(FNodeType::EVALUATION));       // 2
        fMaxObject->addFArg(FArg(1, 0));
        fMaxObject->addFArg(FArg(2, 0));
        fMaxObject->setRootIndex(0);
        MockEvaluator* evaluator = new MockEvaluator();
        fMaxObject->attachEvaluator(1, evaluator);  
        MockEvaluator* evaluator2 = new MockEvaluator();
        fMaxObject->attachEvaluator(2, evaluator2);

        // Setup fMinObject with a MIN node and 2 evaluated nodes
        fMinObject->addFNode(FNode(FNodeType::MIN));
        fMinObject->addFNode(FNode(FNodeType::EVALUATION));
        fMinObject->addFNode(FNode(FNodeType::EVALUATION));

        // Setup fMaxOfNegationObject with a MAX node, a NEGATION node and 2 evaluated nodes.
        fMaxOfNegationObject->addFNode(FNode(FNodeType::NEGATION));
        fMaxOfNegationObject->addFNode(FNode(FNodeType::MAX));
    }

    void TearDown() override {
        fMaxObject.reset();
        fMinObject.reset();
        fMaxOfNegationObject.reset();
    }

    std::unique_ptr<FObject> fMaxObject;
    std::unique_ptr<FObject> fMinObject;
    std::unique_ptr<FObject> fMaxOfNegationObject;   
};




} // namespace e2
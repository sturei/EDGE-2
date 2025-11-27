#include <gtest/gtest.h>
#include "frep/functions.h"
#include "frep/fobject.h"
#include "utils/vec3d.h"
#include <vector>

namespace e2 {

class FEvaluateTest : public ::testing::Test {
protected:
    Vec3d position{1.0, 2.0, 3.0};
    Vec3d _position{0,0,0};         // dummy position for functions that don't use it
    double output;
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

// FMax tests
TEST_F(FEvaluateTest, FMaxEvaluateWithMultipleValues) {
    FMax fmax;
    std::vector<double> args = {1.0, 5.0, 3.0, 2.0};
    
    bool result = fmax.evaluate(_position, args, output);
    
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, 5.0);
}

TEST_F(FEvaluateTest, FMaxEvaluateWithSingleValue) {
    FMax fmax;
    std::vector<double> args = {42.0};
    
    bool result = fmax.evaluate(_position, args, output);
    
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, 42.0);
}

TEST_F(FEvaluateTest, FMaxEvaluateWithEmptyArgs) {
    FMax fmax;
    std::vector<double> args;
    
    bool result = fmax.evaluate(_position, args, output);
    
    EXPECT_FALSE(result);
}

TEST_F(FEvaluateTest, FMaxEvaluateWithNegativeValues) {
    FMax fmax;
    std::vector<double> args = {-5.0, -1.0, -3.0};
    
    bool result = fmax.evaluate(_position, args, output);
    
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, -1.0);
}

// FMin tests
TEST_F(FEvaluateTest, FMinEvaluateWithMultipleValues) {
    FMin fmin;
    std::vector<double> args = {1.0, 5.0, 3.0, 2.0};
    
    bool result = fmin.evaluate(_position, args, output);
    
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, 1.0);
}

TEST_F(FEvaluateTest, FMinEvaluateWithSingleValue) {
    FMin fmin;
    std::vector<double> args = {42.0};
    
    bool result = fmin.evaluate(_position, args, output);
    
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, 42.0);
}

TEST_F(FEvaluateTest, FMinEvaluateWithEmptyArgs) {
    FMin fmin;
    std::vector<double> args;
    
    bool result = fmin.evaluate(_position, args, output);
    
    EXPECT_FALSE(result);
}

// FNegation tests
TEST_F(FEvaluateTest, FNegationEvaluateWithPositiveValue) {
    FNegation fneg;
    std::vector<double> args = {5.0};
    
    bool result = fneg.evaluate(_position, args, output);
    
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, -5.0);
}

TEST_F(FEvaluateTest, FNegationEvaluateWithNegativeValue) {
    FNegation fneg;
    std::vector<double> args = {-3.0};
    
    bool result = fneg.evaluate(_position, args, output);
    
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, 3.0);
}

TEST_F(FEvaluateTest, FNegationEvaluateWithZero) {
    FNegation fneg;
    std::vector<double> args = {0.0};
    
    bool result = fneg.evaluate(_position, args, output);
    
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, 0.0);
}

TEST_F(FEvaluateTest, FNegationEvaluateWithWrongArgCount) {
    FNegation fneg;
    std::vector<double> args = {1.0, 2.0};
    
    bool result = fneg.evaluate(_position, args, output);
    
    EXPECT_FALSE(result);
}

TEST_F(FEvaluateTest, FNegationEvaluateWithEmptyArgs) {
    FNegation fneg;
    std::vector<double> args;
    
    bool result = fneg.evaluate(_position, args, output);
    
    EXPECT_FALSE(result);
}

// FConstant tests
TEST_F(FEvaluateTest, FConstantEvaluateWithPositiveValue) {
    FConstant fconst(42.5);
    std::vector<double> _args = {1.0, 2.0, 3.0}; // args should be ignored
    
    bool result = fconst.evaluate(_position, _args, output);
    
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, 42.5);
}

TEST_F(FEvaluateTest, FConstantEvaluateWithNegativeValue) {
    FConstant fconst(-10.0);
    std::vector<double> _args;
    
    bool result = fconst.evaluate(_position, _args, output);
    
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, -10.0);
}

TEST_F(FEvaluateTest, FConstantEvaluateWithZero) {
    FConstant fconst(0.0);
    std::vector<double> _args;
    
    bool result = fconst.evaluate(_position, _args, output);
    
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, 0.0);
}

TEST_F(FEvaluateTest, FObjectEvaluatesToMax) {
    bool result = evaluate(maxObject, _position, output );
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(output, 42.0);
}

// Print tests
TEST(FEvaluatePrintTest, FMaxPrint) {
    FMax fmax;
    std::ostringstream oss;
    
    fmax.print(oss);
    
    EXPECT_EQ(oss.str(), "FMax");
}

TEST(FEvaluatePrintTest, FMinPrint) {
    FMin fmin;
    std::ostringstream oss;
    
    fmin.print(oss);
    
    EXPECT_EQ(oss.str(), "FMin");
}

TEST(FEvaluatePrintTest, FNegationPrint) {
    FNegation fneg;
    std::ostringstream oss;
    
    fneg.print(oss);
    
    EXPECT_EQ(oss.str(), "FNegation");
}

TEST(FEvaluatePrintTest, FConstantPrint) {
    FConstant fconst(3.14);
    std::ostringstream oss;
    
    fconst.print(oss);
    
    EXPECT_EQ(oss.str(), "FConstant(3.14)");
}



} // namespace e2
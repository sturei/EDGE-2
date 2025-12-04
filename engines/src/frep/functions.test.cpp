#include <gtest/gtest.h>
#include "frep/functions.h"
#include "frep/fobject.h"
#include "frep/frep.fixtures.h" 
#include "utils/vec3d.h"
#include <vector>

using namespace e2;

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

class FSDFTest : public ::testing::Test {
protected:
    void SetUp() override {
        block = FRepFixtures::block(2.0, 2.0, 2.0);
        infiniteExtrudedRectangle = FRepFixtures::infiniteRectangle(Vec3d(-1.0, -1.0, 0.0), Vec3d(1.0, 1.0, 0.0));
        extrudedRectangle = FRepFixtures::cappedRectangle(Vec3d(-1.0, -1.0, 0.0), Vec3d(1.0, 1.0, 0.0), 20.0);
        extrudedRectangleExact = FRepFixtures::extrudedRectangle(Vec3d(-1.0, -1.0, 0.0), Vec3d(1.0, 1.0, 0.0), 20.0);
    }
    void TearDown() override {
        delete block;
        delete infiniteExtrudedRectangle;
        delete extrudedRectangle;
        delete extrudedRectangleExact;
    }
    FObject* block;                       // unit half-side cube centered at origin, represented as 6 half-planes
    FObject* infiniteExtrudedRectangle;   // infinite prism, represented as unit square, extruded
    FObject* extrudedRectangle;           // finite prism, represented as unit square, extruded by 10
    FObject* extrudedRectangleExact;      // finite prism, represented as unit square, extruded by 10, exact SDF
};

TEST_F(FSDFTest, FSDFTestBlockEvaluate) {
    std::array<std::pair<Vec3d, double>, 3> positionAndExpectedSDF = {
        std::make_pair(Vec3d(0.0, 0.0, 0.0), -1.0), // inside the block
        std::make_pair(Vec3d(1.0, 1.0, 1.0), 0.0),  // on the surface
        std::make_pair(Vec3d(2.0, 2.0, 2.0), sqrt(3))   // outside
    };
    for (const auto& [position, expectedSDF] : positionAndExpectedSDF) {
        double sdfOut;  
        bool result = block->evaluate(position, sdfOut);   
        EXPECT_TRUE(result);
        EXPECT_DOUBLE_EQ(sdfOut, expectedSDF);
    }
}

TEST_F(FSDFTest, FSDFTestInfiniteExtrudedRectangleEvaluate) {
    std::array<std::pair<Vec3d, double>, 3> positionAndExpectedSDF = {
        std::make_pair(Vec3d(0.0, 0.0, 0.0), -1.0),    // inside
        std::make_pair(Vec3d(1.0, 1.0, 1.0), 0.0),     // on the surface
        std::make_pair(Vec3d(2.0, 2.0, 2.0), sqrt(2))  // outside
    };
    for (const auto& [position, expectedSDF] : positionAndExpectedSDF) {
        double sdfOut;  

        bool result = infiniteExtrudedRectangle->evaluate(position, sdfOut);   
        EXPECT_TRUE(result);
        EXPECT_DOUBLE_EQ(sdfOut, expectedSDF);
    }
}

TEST_F(FSDFTest, FSDFTestExtrudedRectangleEvaluate) {
    std::array<std::pair<Vec3d, double>, 5> positionAndExpectedSDF = {
        std::make_pair(Vec3d(0.0, 0.0, 1.0), -1.0),     // inside
        std::make_pair(Vec3d(1.0, 1.0, 1.0), 0.0),      // on a vertical edge
        std::make_pair(Vec3d(0.5, 0.0, 11.0), 1.0),     // above the top face
        std::make_pair(Vec3d(2.0, 2.0, 2.0), sqrt(2)),  // outside a vertical edge
        std::make_pair(Vec3d(2.0, 0.0, 11), 1.0)        // outside a horizontal edge

    };
    for (const auto& [position, expectedSDF] : positionAndExpectedSDF) {
        double sdfOut;  
        bool result = extrudedRectangle->evaluate(position, sdfOut);   
        EXPECT_TRUE(result);
        EXPECT_DOUBLE_EQ(sdfOut, expectedSDF);
    }
}

TEST_F(FSDFTest, FSDFTestExtrudedRectangleExactEvaluate) {
    std::array<std::pair<Vec3d, double>, 5> positionAndExpectedSDF = {
        std::make_pair(Vec3d(0.0, 0.0, 1.0), -1.0),     // inside
        std::make_pair(Vec3d(1.0, 1.0, 1.0), 0.0),      // on a vertical edge
        std::make_pair(Vec3d(0.5, 0.0, 11.0), 1.0),     // above the top face
        std::make_pair(Vec3d(2.0, 2.0, 2.0), sqrt(2)),  // outside a vertical edge
        std::make_pair(Vec3d(2.0, 0.0, 11), sqrt(2))        // outside a horizontal edge. This is the key difference to the non-exact version

    };
    for (const auto& [position, expectedSDF] : positionAndExpectedSDF) {
        double sdfOut;  
        bool result = extrudedRectangleExact->evaluate(position, sdfOut);   
        EXPECT_TRUE(result);
        EXPECT_DOUBLE_EQ(sdfOut, expectedSDF);
    }
}


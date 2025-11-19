#include <gtest/gtest.h>
#include "frep/fevaluate.h"
#include "frep/fobject.h"
#include "frep/fsdf.h"
#include "frep/frep.fixtures.h" 
#include "brep/brep.fixtures.h"
#include "utils/vec3d.h"
#include <vector>

using namespace e2;

class FSDFTest : public ::testing::Test {
protected:
    void SetUp() override {
        block = FRepFixtures::block(2.0, 2.0, 2.0);
        sheetRectangle = BRepFixtures::sheetRectangle(Vec3d(-1.0, -1.0, 0.0), Vec3d(1.0, 1.0, 0.0));
        infiniteExtrudedRectangle = FRepFixtures::infiniteExtrudedProfile(*sheetRectangle);
        extrudedRectangle = FRepFixtures::extrudedProfile(*sheetRectangle, 20.0);
        extrudedRectangleExact = FRepFixtures::extrudedProfileExact(*sheetRectangle, 20.0);
    }
    void TearDown() override {
        delete block;
        delete sheetRectangle;
        delete infiniteExtrudedRectangle;
        delete extrudedRectangle;
        delete extrudedRectangleExact;
    }
    FObject* block;                       // unit half-side cube centered at origin, represented as 6 half-planes
    Body* sheetRectangle;                 // unit half-side square in XY plane centered at origin, represented as brep sheet
    FObject* infiniteExtrudedRectangle;   // infinite prism, represented as unit square, extruded
    FObject* extrudedRectangle;           // finite prism, represented as unit square, extruded by 10
    FObject* extrudedRectangleExact;      // finite prism, represented as unit square, extruded by 10, exact SDF
};

TEST_F(FSDFTest, FSDFTestBlockEvaluate) {
    std::array<std::pair<Vec3d, double>, 3> positionAndExpectedSDF = {
        std::make_pair(Vec3d(0.0, 0.0, 0.0), -1.0), // inside the block
        std::make_pair(Vec3d(1.0, 1.0, 1.0), 0.0),  // on the surface
        std::make_pair(Vec3d(2.0, 2.0, 2.0), 1.0)   // outside
    };
    for (const auto& [position, expectedSDF] : positionAndExpectedSDF) {
        double sdfOut;  
        bool result = evaluate(*block, position, sdfOut);   
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

        bool result = evaluate(*infiniteExtrudedRectangle, position, sdfOut);   
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
        bool result = evaluate(*extrudedRectangle, position, sdfOut);   
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
        bool result = evaluate(*extrudedRectangleExact, position, sdfOut);   
        EXPECT_TRUE(result);
        EXPECT_DOUBLE_EQ(sdfOut, expectedSDF);
    }
}


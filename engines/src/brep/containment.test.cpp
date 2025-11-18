#include <gtest/gtest.h>
#include "brep/containment.h"
#include "brep/body.h"
#include "brep/brep.fixtures.h"
#include "utils/vec3d.h"

using namespace e2;

class ContainmentTest : public ::testing::Test {
protected:
    void SetUp() override {
            rectangleFixture = BRepFixtures::sheetRectangle(Vec3d(0,0,0), Vec3d(1,1,0));
    }
    void TearDown() override {
            delete rectangleFixture;
    }
    Body* rectangleFixture;           // a rectangular sheet in the XY plane from (0,0,0) to (1,1,0)
};

TEST_F(ContainmentTest, VertexContainsPosition_ExactMatch_ReturnsTrue) {
    // This test would require a proper Body setup with a vertex
    const Body& body = *rectangleFixture;
    CellIndex vertexIndex = 0;
    Vec3d pos(0.0, 0.0, 0.0); // Exact position of the lower-left vertex
    EXPECT_TRUE(vertexContainsPosition(vertexIndex, body, pos));
}

TEST_F(ContainmentTest, VertexContainsPosition_NoMatch_ReturnsFalse) {
    const Body& body = *rectangleFixture;
    CellIndex vertexIndex = 0;
    Vec3d pos(1.0, 2.0, 3.0);
    EXPECT_FALSE(vertexContainsPosition(vertexIndex, body, pos));
}

TEST_F(ContainmentTest, VertexContainsPosition_InvalidVertex_ReturnsFalse) {
    const Body& body = *rectangleFixture;
    CellIndex invalidVertexIndex = 999;
    Vec3d pos(1.0, 2.0, 3.0);
    EXPECT_FALSE(vertexContainsPosition(invalidVertexIndex, body, pos));
}

TEST_F(ContainmentTest, EdgeContainsPosition_PointOnEdge_ReturnsTrue) {
    const Body& body = *rectangleFixture;
    CellIndex edgeIndex = 4; // lower edge
    Vec3d position(0.5, 0.0, 0.0); // Point on edge
    EXPECT_TRUE(edgeContainsPosition(edgeIndex, body, position));
}


TEST_F(ContainmentTest, EdgeContainsPosition_PointNotOnEdge_ReturnsFalse) {
    const Body& body = *rectangleFixture;
    CellIndex edgeIndex = 4; // lower edge
    Vec3d position(10.0, 10.0, 10.0); // Point far from edge
    EXPECT_FALSE(edgeContainsPosition(edgeIndex, body, position));
}


TEST_F(ContainmentTest, EdgeContainsPosition_PointOnCurveButOutsideBounds_ReturnsFalse) {
    const Body& body = *rectangleFixture;
    CellIndex edgeIndex = 4; // lower edge
    Vec3d position(-1.0, 0.0, 0.0); // Point on curve extension but outside bounds
    EXPECT_FALSE(edgeContainsPosition(edgeIndex, body, position));
}

TEST_F(ContainmentTest, FaceContainsPosition_PointInsideFace_ReturnsTrue) {
    const Body& body = *rectangleFixture;
    CellIndex faceIndex = 8;
    Vec3d position(0.5, 0.5, 0.0); // Point inside face
    EXPECT_TRUE(faceContainsPosition(faceIndex, body, position));
}

TEST_F(ContainmentTest, FaceContainsPosition_PointOutsideFace_ReturnsFalse) {
    const Body& body = *rectangleFixture;
    CellIndex faceIndex = 8;
    Vec3d position(10.0, 10.0, 0.0); // Point outside face
    EXPECT_FALSE(faceContainsPosition(faceIndex, body, position));
}

TEST_F(ContainmentTest, FaceContainsPosition_EvenNumberOfCrossings_ReturnsFalse) {
    const Body& body = *rectangleFixture;
    CellIndex faceIndex = 8;
    Vec3d position(-1.0, 0.5, 0.0);
    EXPECT_FALSE(faceContainsPosition(faceIndex, body, position));
}

TEST_F(ContainmentTest, FaceContainsPosition_OddNumberOfCrossings_ReturnsTrue) {
    const Body& body = *rectangleFixture;
    CellIndex faceIndex = 8;
    Vec3d position(0.5, 0.5, 0.0);
    EXPECT_TRUE(faceContainsPosition(faceIndex, body, position));
}

// Note: cellContainsPosition and bodyContainsPosition are TODO functions
// Tests would be added once implementation is complete


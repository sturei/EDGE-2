#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "brep/brep.fixtures.h"
#include "brep/navigate.h"

using namespace e2;

class NavigateTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // An empty body - no cells
            emptyBodyFixture = BRepFixtures::createEmptyBody();

            // An acorn body - one cell, which is a point
            acornBodyFixture = BRepFixtures::createAcornBody(Vec3d(1,2,3));

            wireRectangleFixture = BRepFixtures::createWireRectangle(Vec3d(0,0,0), Vec3d(1,1,0));   

            sheetRectangleFixture = BRepFixtures::createSheetRectangle(Vec3d(0,0,0), Vec3d(1,1,0));

        };

        void TearDown() override {
            delete emptyBodyFixture;
            delete acornBodyFixture;
            delete wireRectangleFixture;
            delete sheetRectangleFixture;
        }
        Body* emptyBodyFixture;
        Body* acornBodyFixture;
        Body* wireRectangleFixture;
        Body* sheetRectangleFixture;
};

TEST_F(NavigateTest, GetKSkeleton) {  
    Body* body = sheetRectangleFixture;

    auto faces = getKSkeleton(2, *body);
    EXPECT_EQ(faces.size(), 1); // one face

    auto edges = getKSkeleton(1, *body);
    EXPECT_EQ(edges.size(), 4); // four edges

    auto vertices = getKSkeleton(0, *body);
    EXPECT_EQ(vertices.size(), 4); // four vertices

}

TEST_F(NavigateTest, getKBoundary) {  
    Body* body = sheetRectangleFixture;

    auto faces = getKSkeleton(2, *body);
    auto edges = getKSkeleton(1, *body);
    auto vertices = getKSkeleton(0, *body);

    auto faceBoundary = getKBoundary(1, faces[0], *body);
    EXPECT_EQ(faceBoundary.size(), 4); // four edges in face boundary
    
    auto edgeBoundary = getKBoundary(0, edges[0], *body);
    EXPECT_EQ(edgeBoundary.size(), 2); // two vertices in edge boundary

}   

TEST_F(NavigateTest, getKStar) {  
    Body* body = sheetRectangleFixture;

    auto faces = getKSkeleton(2, *body);
    auto edges = getKSkeleton(1, *body);
    auto vertices = getKSkeleton(0, *body);

    auto edgeStar = getKStar(2, edges[0], *body);
    EXPECT_EQ(edgeStar.size(), 1); // one face in edge star

    auto vertexStar = getKStar(1, vertices[0], *body);
    EXPECT_EQ(vertexStar.size(), 2); // two edges are incident on each vertex

}   

TEST_F(NavigateTest, GetEdgesOfFace) {
    Body* body = sheetRectangleFixture;
    auto faces = getKSkeleton(2, *body);
    ASSERT_EQ(faces.size(), 1);
    auto edges = getEdgesOfFace(faces[0], *body);
    EXPECT_EQ(edges.size(), 4); // four edges in the face
}       

TEST_F(NavigateTest, GetVerticesOfEdge) {
    Body* body = sheetRectangleFixture;
    auto faces = getKSkeleton(2, *body);
    ASSERT_EQ(faces.size(), 1);
    auto edges = getEdgesOfFace(faces[0], *body);
    ASSERT_EQ(edges.size(), 4);
    auto vertices = getVerticesOfEdge(edges[0].first, *body);
    EXPECT_EQ(vertices.size(), 2); // two vertices in an edge
}

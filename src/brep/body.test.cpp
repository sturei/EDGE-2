#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "brep/body.h"

using namespace e2;

struct BodyFixture {
    std::vector<Cell> cells;
    std::vector<Cocell> cocells;
};

class BodyTest : public ::testing::Test {
    protected:
        void SetUp() override {

            // An empty body - no cells
            emptyBodyFixture = new BodyFixture();

            // An acorn body - one cell, which is a point
            acornBodyFixture = new BodyFixture{
                {
                    Cell(Point3d(Vec3d(1,2,3)))
                },
                {}
            };
        };

        void TearDown() override {
            delete emptyBodyFixture;
            delete acornBodyFixture;
        }
        BodyFixture* emptyBodyFixture;
        BodyFixture* acornBodyFixture;
};

TEST(CellTest, CellDefaultConstructor) {
    Cell cell;
    // Cell has a default Geom3d at origin. Perhaps in future it will have rubber geometry by default.
    EXPECT_EQ(cell.support().position().x(), 0);
    
    // printf("sizeof(Cell): %zu\n", sizeof(Cell));

};

TEST(CellTest, CellParameterizedConstructor) {
    Point3d pt(Vec3d(4, 5, 6));
    Cell cell(pt);
    EXPECT_EQ(cell.support().position().x(), 4);
    EXPECT_EQ(cell.support().position().y(), 5);
    EXPECT_EQ(cell.support().position().z(), 6);
};

TEST(CocellTest, CocellDefaultConstructor) {
    Cocell cocell;
    // Default cocell is not connected to any cells.
    EXPECT_EQ(cocell.sense(), 0);
    EXPECT_EQ(cocell.starCell(), -1);
    EXPECT_EQ(cocell.boundaryCell(), -1);

    // printf("sizeof(Cocell): %zu\n", sizeof(Cocell));

};

TEST(CocellTest, CocellParameterizedConstructor) {
    int starCell = 2;
    int boundaryCell = 3;
    int sense = 1;
    Cocell cocell(starCell, boundaryCell, sense);
    EXPECT_EQ(cocell.sense(), sense);
    EXPECT_EQ(cocell.starCell(), starCell);
    EXPECT_EQ(cocell.boundaryCell(), boundaryCell);
};

TEST_F(BodyTest, BodyDefaultConstructor) {
    Body body;
    EXPECT_EQ(body.cells().size(), 0);
    EXPECT_EQ(body.cocells().size(), 0);
    EXPECT_EQ(body.graphNeedsUpdate(), false);
}

TEST_F(BodyTest, BodyParameterizedConstructor) {
    // circle with one interior point
    std::vector<Cell> cells = { 
        Cell(Circle3d(Vec3d(0, 0, 0), Vec3d(0, 0, 1), 2)), 
        Cell(Point3d(Vec3d(2, 0, 0))) 
    };
    std::vector<Cocell> cocells = { Cocell(0, 1, 0) };
    Body body(cells, cocells);

    EXPECT_EQ(body.cells().size(), 2);
    EXPECT_EQ(body.cocells().size(), 1);
    EXPECT_EQ(body.cells()[0].support().position(), Vec3d(0,0,0));
    EXPECT_EQ(body.cells()[1].support().position(), Vec3d(2,0,0));
    EXPECT_EQ(body.cocells()[0].starCell(), 0);
    EXPECT_EQ(body.cocells()[0].boundaryCell(), 1);
    EXPECT_EQ(body.graphNeedsUpdate(), false);
}

TEST_F(BodyTest, AddCell) {
    Body body;
    Cell cell(Point3d(Vec3d(7, 8, 9)));
    body.addCell(cell);
    EXPECT_EQ(body.cells().size(), 1);
    EXPECT_EQ(body.cells()[0].support().position().x(), 7);
    EXPECT_EQ(body.graphNeedsUpdate(), true);
}

TEST_F(BodyTest, AddCocell) {
    // circle with one interior point
    std::vector<Cell> cells = { 
        Cell(Circle3d(Vec3d(0, 0, 0), Vec3d(0, 0, 1), 2)), 
        Cell(Point3d(Vec3d(2, 0, 0))) 
    };
    std::vector<Cocell> cocells = { Cocell(0, 1, 0) };
    Body body(cells);
    body.addCocell(cocells[0]);

    EXPECT_EQ(body.cocells().size(), 1);
    EXPECT_EQ(body.cocells()[0].starCell(), 0);
    EXPECT_EQ(body.cocells()[0].boundaryCell(), 1);
    EXPECT_EQ(body.cocells()[0].sense(), 0);
    EXPECT_EQ(body.graphNeedsUpdate(), true);
}

TEST_F(BodyTest, BodyFromFixtureEmpty) {
    Body body(emptyBodyFixture->cells, emptyBodyFixture->cocells);
    EXPECT_EQ(body.cells().size(), 0);
    EXPECT_EQ(body.cocells().size(), 0);
    EXPECT_EQ(body.graphNeedsUpdate(), false);
}

TEST_F(BodyTest, BodyFromFixtureAcorn) {
    Body body(acornBodyFixture->cells, acornBodyFixture->cocells);
    EXPECT_EQ(body.cells().size(), 1);
    EXPECT_EQ(body.cells()[0].support().position(), Vec3d(1,2,3));
    EXPECT_EQ(body.cocells().size(), 0);
    EXPECT_EQ(body.graphNeedsUpdate(), false);
}

TEST_F(BodyTest, GraphFromFixtureEmpty) {
    Body body(emptyBodyFixture->cells, emptyBodyFixture->cocells);
    Graph graph = body.graph();
    EXPECT_EQ(graph.numVertices(), 0);
}

TEST_F(BodyTest, GraphFromFixtureAcorn) {
    Body body(acornBodyFixture->cells, acornBodyFixture->cocells);
    Graph graph = body.graph();
    EXPECT_EQ(graph.numVertices(), 1);
    Graph::VertexView v = graph.vertex(0);
    EXPECT_EQ(v.outDegree, 0);
    EXPECT_EQ(v.inDegree, 0);
}










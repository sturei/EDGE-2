#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "brep/body.h"
#include "brep/brep.fixtures.h"

using namespace e2;

class BodyTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // An empty body - no cells
            emptyBodyFixture = BRepFixtures::emptyBody();

            // An acorn body - one cell, which is a point
            acornBodyFixture = BRepFixtures::acornBody(Vec3d(1,2,3));
        };

        void TearDown() override {
            delete emptyBodyFixture;
            delete acornBodyFixture;
        }
        Body* emptyBodyFixture;
        Body* acornBodyFixture;
};

TEST(CellTest, CellDefaultConstructor) {
    Cell cell;
    // Cell has a default Geom3d at origin. Perhaps in future it will have rubber geometry by default.
    EXPECT_EQ(cell.support().position().x(), 0);
};

TEST(CellTest, CellParameterizedConstructor) {
    Vec3d pt(4, 5, 6);
    Cell cell(pt);
    EXPECT_EQ(cell.isActive(), true);
    EXPECT_EQ(cell.support().type(), Geom3dType::POINT);
    EXPECT_EQ(cell.support().position().x(), 4);
    EXPECT_EQ(cell.support().position().y(), 5);
    EXPECT_EQ(cell.support().position().z(), 6);
};

TEST(CellTest, CellTest_OstreamOperator) {
    Vec3d pt(4, 5, 6);
    Cell cell(pt);
    std::ostringstream oss;
    oss << cell;
    std::string cellStr = oss.str();

    // std::cout << "Cell output:\n" << cellStr << std::endl; //--- IGNORE ---

    // Check that the output string contains some expected substrings
    EXPECT_NE(cellStr.find("active=1"), std::string::npos);
    EXPECT_NE(cellStr.find("pos=(4, 5, 6)"), std::string::npos);
};

TEST(CocellTest, CocellDefaultConstructor) {
    Cocell cocell;
    // Default cocell is not connected to any cells.
    EXPECT_EQ(cocell.sense(), 0);
    EXPECT_EQ(cocell.starCell(), -1);
    EXPECT_EQ(cocell.boundaryCell(), -1);
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

TEST(CocellTest, CocellTest_OstreamOperator) {
    int starCell = 2;
    int boundaryCell = 3;
    int sense = -1;
    Cocell cocell(starCell, boundaryCell, sense);
    std::ostringstream oss;
    oss << cocell;
    std::string cocellStr = oss.str();

    //std::cout << "Cocell output:\n" << cocellStr << std::endl; --- IGNORE ---

    // Check that the output string contains some expected substrings
    EXPECT_NE(cocellStr.find("starCell=2"), std::string::npos);
    EXPECT_NE(cocellStr.find("boundaryCell=3"), std::string::npos);
    EXPECT_NE(cocellStr.find("sense=-1"), std::string::npos);
};

TEST_F(BodyTest, BodyDefaultConstructor) {
    Body body;
    EXPECT_EQ(body.cells().size(), 0);
    EXPECT_EQ(body.cocells().size(), 0);
    EXPECT_EQ(body.graphNeedsUpdate(), false);
}

TEST_F(BodyTest, BodyCopyConstructor) {
    // circle with one interior point
    std::vector<Cell> cells = { 
        Cell(Cir3d(Vec3d(0, 0, 0), 2, Vec3d(0, 0, 1))), 
        Cell(Vec3d(2, 0, 0)) 
    };
    std::vector<Cocell> cocells = { Cocell(0, 1, 0) };
    Body originalBody(cells, cocells);
    Body copiedBody(originalBody);

    EXPECT_EQ(copiedBody.cells().size(), 2);
    EXPECT_EQ(copiedBody.cocells().size(), 1);
    EXPECT_EQ(copiedBody.cells()[0].support().position(), Vec3d(0,0,0));
    EXPECT_EQ(copiedBody.cells()[1].support().position(), Vec3d(2,0,0));
    EXPECT_EQ(copiedBody.cocells()[0].starCell(), 0);
    EXPECT_EQ(copiedBody.cocells()[0].boundaryCell(), 1);
    EXPECT_EQ(copiedBody.graphNeedsUpdate(), false);
}

TEST_F(BodyTest, BodyParameterizedConstructor) {
    // circle with one interior point
    std::vector<Cell> cells = { 
        Cell(Cir3d(Vec3d(0, 0, 0), 2, Vec3d(0, 0, 1))), 
        Cell(Vec3d(2, 0, 0)) 
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
    Cell cell(Vec3d(7, 8, 9));
    body.addCell(cell);
    EXPECT_EQ(body.cells().size(), 1);
    EXPECT_EQ(body.cells()[0].support().position().x(), 7);
    EXPECT_EQ(body.graphNeedsUpdate(), true);
}

TEST_F(BodyTest, AddCocell) {
    // circle with one interior point
    std::vector<Cell> cells = { 
        Cell(Cir3d(Vec3d(0, 0, 0), 2, Vec3d(0, 0, 1))), 
        Cell(Vec3d(2, 0, 0)) 
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
    Body* body = emptyBodyFixture;
    EXPECT_EQ(body->cells().size(), 0);
    EXPECT_EQ(body->cocells().size(), 0);
    EXPECT_EQ(body->graphNeedsUpdate(), false);
}

TEST_F(BodyTest, BodyFromFixtureAcorn) {
    Body* body = acornBodyFixture;
    EXPECT_EQ(body->cells().size(), 1);
    EXPECT_EQ(body->cells()[0].support().position(), Vec3d(1,2,3));
    EXPECT_EQ(body->cocells().size(), 0);
    EXPECT_EQ(body->graphNeedsUpdate(), false);
}

TEST_F(BodyTest, GraphFromFixtureEmpty) {
    const Graph& graph = emptyBodyFixture->graph();
    EXPECT_EQ(graph.numNodes(), 0);
}

TEST_F(BodyTest, GraphFromFixtureAcorn) {
    Body* body = acornBodyFixture;
    const Graph& graph = body->graph();
    EXPECT_EQ(graph.numNodes(), 1);
    Graph::NodeView v = graph.node(0);
    EXPECT_EQ(v.outDegree, 0);
    EXPECT_EQ(v.inDegree, 0);
}

TEST_F(BodyTest, OstreamOutputOperator) {
    Body* body = acornBodyFixture;
    std::ostringstream oss;
    oss << *body;
    std::string bodyStr = oss.str();

    // std::cout << "Body output:\n" << bodyStr << std::endl; // --- IGNORE ---

    // Check that the output string contains some expected substrings
    EXPECT_NE(bodyStr.find("Body with 1 cells and 0 cocells."), std::string::npos);
}











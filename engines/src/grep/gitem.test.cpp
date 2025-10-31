
#include <gtest/gtest.h>
#include "grep/gitem.h"

using namespace e2;

TEST(GPointTest, BasicFunctionality) {
    GPoint point(5);
    EXPECT_EQ(point.type(), GItemType::GPOINT);
    EXPECT_EQ(point.size(), 5);
    EXPECT_EQ(point.toString(), "GPoint(size=5)");
}

TEST(GLineTest, BasicFunctionality) {
    GLine line(10);
    EXPECT_EQ(line.type(), GItemType::GLINE);
    EXPECT_EQ(line.length(), 10);
    EXPECT_EQ(line.toString(), "GLine(length=10)");
}

TEST(GPlaneTest, BasicFunctionality) {
    GPlane plane(10, 20);
    EXPECT_EQ(plane.type(), GItemType::GPLANE);
    EXPECT_EQ(plane.width(), 10);
    EXPECT_EQ(plane.height(), 20);
    EXPECT_EQ(plane.toString(), "GPlane(width=10, height=20)");
}

TEST(GSphereTest, BasicFunctionality) {
    GSphere sphere(15);
    EXPECT_EQ(sphere.type(), GItemType::GSPHERE);
    EXPECT_EQ(sphere.radius(), 15);
    EXPECT_EQ(sphere.toString(), "GSphere(radius=15)");
}

TEST(GBlockTest, BasicFunctionality) {
    GBlock block(5, 10, 15);
    EXPECT_EQ(block.type(), GItemType::GBLOCK);
    EXPECT_EQ(block.width(), 5);
    EXPECT_EQ(block.height(), 10);
    EXPECT_EQ(block.depth(), 15);
    EXPECT_EQ(block.toString(), "GBlock(width=5, height=10, depth=15)");
}


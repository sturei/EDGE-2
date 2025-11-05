#include "utils/pla3d.h"
#include <gtest/gtest.h>

using namespace e2;

// Test default constructor
TEST(Pla3dTest, DefaultConstructor) {
    Pla3d plane;
    EXPECT_TRUE(plane.position() == Vec3d(0,0,0));
    EXPECT_TRUE(plane.normal() == Vec3d(0,0,1));
}

// Test parameterized constructor
TEST(Pla3dTest, ParameterizedConstructor) {
    Vec3d position(1, 2, 3);
    Vec3d normal(0, 1, 0);
    Pla3d plane(position, normal);
    EXPECT_TRUE(plane.position() == position);
    EXPECT_TRUE(plane.normal() == normal);
}

// Test copy constructor and assignment operator
TEST(Pla3dTest, CopyAndAssignment) {
    Vec3d position(4, 5, 6);
    Vec3d normal(1, 0, 0);
    Pla3d plane1(position, normal);
    Pla3d plane2 = plane1; // Copy constructor
    EXPECT_TRUE(plane2.position() == position);
    EXPECT_TRUE(plane2.normal() == normal);

    Pla3d plane3;
    plane3 = plane1; // Assignment operator
    EXPECT_TRUE(plane3.position() == position);
    EXPECT_TRUE(plane3.normal() == normal);
}

// Test equality operator
TEST(Pla3dTest, EqualityOperator) {
    Vec3d position(7, 8, 9);
    Vec3d normal(0, 0, 1);
    Pla3d plane1(position, normal);
    Pla3d plane2(position, normal);
    Pla3d plane3(Vec3d(1, 2, 3), Vec3d(1, 0, 0));
    EXPECT_TRUE(plane1 == plane2);
    EXPECT_FALSE(plane1 == plane3);
}

// Test stream output operator
TEST(Pla3dTest, StreamOutputOperator) {
    Vec3d position(1, 2, 3);
    Vec3d normal(0, 1, 0);
    Pla3d plane(position, normal);
    std::ostringstream oss;
    oss << plane;
    EXPECT_EQ(oss.str(), "Position: (1, 2, 3), Normal: (0, 1, 0)");
}


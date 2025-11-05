#include <gtest/gtest.h>
#include "utils/ray3d.h"
using namespace e2;

// Test default constructor
TEST(Ray3dTest, DefaultConstructor) {
    Ray3d ray;
    EXPECT_TRUE(ray.position() == Vec3d(0,0,0));
    EXPECT_TRUE(ray.direction() == Vec3d(0,0,1));
}

// Test parameterized constructor
TEST(Ray3dTest, ParameterizedConstructor) {
    Vec3d position(1, 2, 3);
    Vec3d direction(0, 1, 0);
    Ray3d ray(position, direction);
    EXPECT_TRUE(ray.position() == position);
    EXPECT_TRUE(ray.direction() == direction);
}

// Test copy constructor and assignment operator
TEST(Ray3dTest, CopyAndAssignment) {
    Vec3d position(4, 5, 6);
    Vec3d direction(1, 0, 0);
    Ray3d ray1(position, direction);
    Ray3d ray2 = ray1; // Copy constructor
    EXPECT_TRUE(ray2.position() == position);
    EXPECT_TRUE(ray2.direction() == direction);

    Ray3d ray3;
    ray3 = ray1; // Assignment operator
    EXPECT_TRUE(ray3.position() == position);
    EXPECT_TRUE(ray3.direction() == direction);
}

// Test equality operator
TEST(Ray3dTest, EqualityOperator) {
    Vec3d position(7, 8, 9);
    Vec3d direction(0, 0, 1);
    Ray3d ray1(position, direction);
    Ray3d ray2(position, direction);
    Ray3d ray3(Vec3d(1, 2, 3), Vec3d(1, 0, 0));
    EXPECT_TRUE(ray1 == ray2);
    EXPECT_FALSE(ray1 == ray3);
}

// Test stream output operator
TEST(Ray3dTest, StreamOutputOperator) {
    Vec3d origin(1, 2, 3);
    Vec3d direction(0, 1, 0);
    Ray3d ray(origin, direction);
    std::ostringstream oss;
    oss << ray;
    EXPECT_EQ(oss.str(), "Position: (1, 2, 3), Direction: (0, 1, 0)");
}


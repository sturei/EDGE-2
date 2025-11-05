#include <gtest/gtest.h>
#include "utils/sph3d.h"

using namespace e2;

// Test default constructor
TEST(Sph3dTest, DefaultConstructor) {
    Sph3d sphere;
    EXPECT_TRUE(sphere.center() == Vec3d(0,0,0));
    EXPECT_DOUBLE_EQ(sphere.radius(), 1.0);
}

// Test parameterized constructor
TEST(Sph3dTest, ParameterizedConstructor) {
    Vec3d center(1, 2, 3);
    double radius = 5.0;
    Sph3d sphere(center, radius);
    EXPECT_TRUE(sphere.center() == center);
    EXPECT_EQ(sphere.radius(), radius);
}

// Test copy constructor and assignment operator
TEST(Sph3dTest, CopyAndAssignment) {
    Vec3d center(4, 5, 6);
    double radius = 2.5;
    Sph3d sphere1(center, radius);
    Sph3d sphere2 = sphere1; // Copy constructor
    EXPECT_TRUE(sphere2.center() == center);
    EXPECT_EQ(sphere2.radius(), radius);

    Sph3d sphere3;
    sphere3 = sphere1; // Assignment operator
    EXPECT_TRUE(sphere3.center() == center);
    EXPECT_EQ(sphere3.radius(), radius);
}

// Test equality operator
TEST(Sph3dTest, EqualityOperator) {
    Vec3d center(7, 8, 9);
    double radius = 3.0;
    Sph3d sphere1(center, radius);
    Sph3d sphere2(center, radius);
    Sph3d sphere3(Vec3d(1, 2, 3), 1.0);
    EXPECT_TRUE(sphere1 == sphere2);
    EXPECT_FALSE(sphere1 == sphere3);
}

// Test stream output operator
TEST(Sph3dTest, StreamOutputOperator) {
    Vec3d center(1, 2, 3);
    double radius = 4.0;
    Sph3d sphere(center, radius);
    std::ostringstream oss;
    oss << sphere;
    EXPECT_EQ(oss.str(), "Center: (1, 2, 3), Radius: 4");
}


#include <gtest/gtest.h>
#include "utils/cir3d.h"

using namespace e2;

// Test default constructor
TEST(Cir3dTest, DefaultConstructor) {
    Cir3d circle;
    EXPECT_TRUE(circle.center() == Vec3d(0,0,0));
    EXPECT_EQ(circle.radius(), 1.0);
    EXPECT_TRUE(circle.normal() == Vec3d(0,0,1));
}

// Test parameterized constructor
TEST(Cir3dTest, ParameterizedConstructor) {
    Vec3d center(1, 2, 3);
    double radius = 5.0;
    Vec3d normal(0, 1, 0);
    Cir3d circle(center, radius, normal);
    EXPECT_TRUE(circle.center() == center);
    EXPECT_EQ(circle.radius(), radius);
    EXPECT_TRUE(circle.normal() == normal);
}

// Test local x axis calculation
TEST(Cir3dTest, LocalXAxis) {
    Vec3d center(1, 2, 3);
    double radius = 5.0;

    // Test with specified x axis
    Vec3d normal1(0, 0, 1);
    Vec3d xaxis1(1, 0, 0);
    Cir3d circle1(center, radius, normal1, xaxis1);
    EXPECT_TRUE(circle1.xaxis() == xaxis1);
    EXPECT_TRUE(circle1.yaxis() == normal1.cross(xaxis1));

    // Test with arbitrary normal vector
    Vec3d normal = Vec3d(1, 1, 1).normalize();
    Cir3d circle(center, radius, normal);

    std::cout << "*******Circle: " << circle << std::endl;

    Vec3d localX = circle.xaxis();
    // Local X should be perpendicular to normal
    EXPECT_NEAR(localX.dot(normal), 0.0, 1e-10);
    // Local X should be perpendicular to local Y
    Vec3d localY = circle.yaxis();
    EXPECT_NEAR(localX.dot(localY), 0.0, 1e-10);
    // Local Y should be perpendicular to normal
    EXPECT_NEAR(localY.dot(normal), 0.0, 1e-10);
    // Local Y should be unit length
    EXPECT_NEAR(localY.magnitude(), 1.0, 1e-10);       
    // Local X should be unit length
    EXPECT_NEAR(localX.magnitude(), 1.0, 1e-10);
}

// Test copy constructor and assignment operator
TEST(Cir3dTest, CopyAndAssignment) {
    Vec3d center(4, 5, 6);
    double radius = 3.5;
    Vec3d normal(1, 0, 0);
    Cir3d circle1(center, radius, normal);
    Cir3d circle2 = circle1; // Copy constructor
    EXPECT_TRUE(circle2.center() == center);
    EXPECT_EQ(circle2.radius(), radius);
    EXPECT_TRUE(circle2.normal() == normal);

    Cir3d circle3;
    circle3 = circle1; // Assignment operator
    EXPECT_TRUE(circle3.center() == center);
    EXPECT_EQ(circle3.radius(), radius);
    EXPECT_TRUE(circle3.normal() == normal);
}

// Test equality operator
TEST(Cir3dTest, EqualityOperator) {
    Vec3d center(7, 8, 9);
    double radius = 2.5;
    Vec3d normal(0, 0, 1);
    Cir3d circle1(center, radius, normal);
    Cir3d circle2(center, radius, normal);
    Cir3d circle3(Vec3d(1, 2, 3), 4.0, Vec3d(1, 0, 0));
    EXPECT_TRUE(circle1 == circle2);
    EXPECT_FALSE(circle1 == circle3);
}

// Test stream output operator
TEST(Cir3dTest, StreamOutputOperator) {
    Vec3d center(1, 2, 3);
    double radius = 4.0;
    Vec3d normal(0, 1, 0);
    Cir3d circle(center, radius, normal);
    std::ostringstream oss;
    oss << circle;
    EXPECT_EQ(oss.str(), "Center: (1, 2, 3), Radius: 4, Normal: (0, 1, 0), XAxis: (0, 0, -1), YAxis: (-1, 0, 0)");
}


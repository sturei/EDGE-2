#include "utils/vec3d.h"
#include <gtest/gtest.h>

using namespace e2;

TEST(Vec3dTest, DefaultConstructor) {
    Vec3d v;
    EXPECT_EQ(v.x(), 0.0);
    EXPECT_EQ(v.y(), 0.0);
    EXPECT_EQ(v.z(), 0.0);
}

TEST(Vec3dTest, ParameterizedConstructor) {
    Vec3d v(1.0, 2.0, 3.0);
    EXPECT_EQ(v.x(), 1.0);
    EXPECT_EQ(v.y(), 2.0);
    EXPECT_EQ(v.z(), 3.0);
}
TEST(Vec3dTest, EqualityOperator) {
    Vec3d v1(1.0, 2.0, 3.0);
    Vec3d v2(1.0, 2.0, 3.0);
    Vec3d v3(3.0, 2.0, 1.0);
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
}
TEST(Vec3dTest, StreamOutputOperator) {
    Vec3d v(1.0, 2.0, 3.0);
    std::ostringstream oss;
    oss << v;
    EXPECT_EQ(oss.str(), "(1, 2, 3)");
}
TEST(Vec3dTest, AdditionOperator) {
    Vec3d v1(1.0, 2.0, 3.0);
    Vec3d v2(4.0, 5.0, 6.0);
    Vec3d result = v1 + v2;
    EXPECT_EQ(result.x(), 5.0);
    EXPECT_EQ(result.y(), 7.0);
    EXPECT_EQ(result.z(), 9.0);
}

TEST(Vec3dTest, SubtractionOperator) {
    Vec3d v1(4.0, 5.0, 6.0);
    Vec3d v2(1.0, 2.0, 3.0);
    Vec3d result = v1 - v2;
    EXPECT_EQ(result.x(), 3.0);
    EXPECT_EQ(result.y(), 3.0);
    EXPECT_EQ(result.z(), 3.0);
}

TEST(Vec3dTest, ScalarMultiplication) {
    Vec3d v(1.0, 2.0, 3.0);
    Vec3d result = v * 2.0;
    EXPECT_EQ(result.x(), 2.0);
    EXPECT_EQ(result.y(), 4.0);
    EXPECT_EQ(result.z(), 6.0);
}

TEST(Vec3dTest, DotProduct) {
    Vec3d v1(1.0, 2.0, 3.0);
    Vec3d v2(4.0, 5.0, 6.0);
    double result = v1.dot(v2);
    EXPECT_EQ(result, 32.0);
}

TEST(Vec3dTest, CrossProduct) {
    Vec3d v1(1.0, 0.0, 0.0);
    Vec3d v2(0.0, 1.0, 0.0);
    Vec3d result = v1.cross(v2);
    EXPECT_EQ(result.x(), 0.0);
    EXPECT_EQ(result.y(), 0.0);
    EXPECT_EQ(result.z(), 1.0);
}

TEST(Vec3dTest, Magnitude) {
    Vec3d v(3.0, 4.0, 0.0);
    double result = v.magnitude();
    EXPECT_EQ(result, 5.0);
}

TEST(Vec3dTest, Normalize) {
    Vec3d v(3.0, 4.0, 0.0);
    Vec3d result = v.normalize();
    EXPECT_DOUBLE_EQ(result.x(), 0.6);
    EXPECT_DOUBLE_EQ(result.y(), 0.8);
    EXPECT_DOUBLE_EQ(result.z(), 0.0);
}



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


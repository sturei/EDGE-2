#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils/evaluate.h"

using namespace e2;

class EvaluateTest : public ::testing::Test {
    protected:
        void SetUp() override {
            ray = Ray3d(Vec3d(1,2,3), Vec3d(0,0,1));
        };

        void TearDown() override {
        }
        Ray3d ray;
        bool isNearlyEqual(const Vec3d& v1, const Vec3d& v2, double tol=1e-6) {
            return (std::abs(v1.x() - v2.x()) < tol) &&
                   (std::abs(v1.y() - v2.y()) < tol) &&
                   (std::abs(v1.z() - v2.z()) < tol);
        }   
};

TEST_F(EvaluateTest, EvaluateRayPoint) {
    double t = 5.0;
    Vec3d point = evaluate(ray, t);
    EXPECT_DOUBLE_EQ(point.x(), 1);
    EXPECT_DOUBLE_EQ(point.y(), 2);
    EXPECT_DOUBLE_EQ(point.z(), 8);
};

TEST_F(EvaluateTest, EvaluateRayDerivs) {
    double t = 5.0;
    EvaluationResult result = evaluate(ray, t, 2);
    EXPECT_EQ(result.nderivsOut, 2);
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[0], Vec3d(1,2,8)));
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[1], Vec3d(0,0,1)));
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[2], Vec3d(0,0,0)));
};

TEST_F(EvaluateTest, EvaluateRayExcessDerivs) {
    double t = 5.0;
    EvaluationResult result = evaluate(ray, t, 5); // request more than 3 derivatives
    EXPECT_EQ(result.nderivsOut, 3); // should only return up to 3
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[0], Vec3d(1,2,8)));
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[1], Vec3d(0,0,1)));
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[2], Vec3d(0,0,0)));
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[3], Vec3d(0,0,0)));
};












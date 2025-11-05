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

TEST_F(EvaluateTest, EvaluateCirclePoint) {
    // Create a circle with center (1,0,0), radius 2, in XY plane
    Vec3d center(1, 0, 0);
    Vec3d xaxis(1, 0, 0);
    Vec3d yaxis(0, 1, 0);
    Vec3d normal = xaxis.cross(yaxis);
    double radius = 2.0;
    Cir3d circle(center, radius, normal, xaxis);
    
    // Test at t = 0 (should be at (3,0,0))
    Vec3d point = evaluate(circle, 0.0);
    EXPECT_TRUE(isNearlyEqual(point, Vec3d(3, 0, 0)));

    // Test at t = π/2 (should be at (1,2,0))
    point = evaluate(circle, M_PI/2);
    EXPECT_TRUE(isNearlyEqual(point, Vec3d(1, 2, 0)));

    // Test at t = π (should be at (-1,0,0))
    point = evaluate(circle, M_PI);
    EXPECT_TRUE(isNearlyEqual(point, Vec3d(-1, 0, 0)));
}

TEST_F(EvaluateTest, EvaluateCircleDerivs) {
    Vec3d center(0, 0, 0);
    Vec3d xaxis(1, 0, 0);
    Vec3d yaxis(0, 1, 0);
    Vec3d normal = xaxis.cross(yaxis);
    double radius = 2.0;
    Cir3d circle(center, radius, normal, xaxis);
    
    // Test derivatives at t = 0
    EvaluationResult result = evaluate(circle, 0.0, 3);
    EXPECT_EQ(result.nderivsOut, 3);
    
    // Position: center + radius*cos(0)*X + radius*sin(0)*Y = (2,0,0)
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[0], Vec3d(2, 0, 0)));
    
    // First derivative: -radius*sin(0)*X + radius*cos(0)*Y = (0,2,0)
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[1], Vec3d(0, 2, 0)));
    
    // Second derivative: -radius*cos(0)*X - radius*sin(0)*Y = (-2,0,0)
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[2], Vec3d(-2, 0, 0)));
    
    // Third derivative: radius*sin(0)*X - radius*cos(0)*Y = (0,-2,0)
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[3], Vec3d(0, -2, 0)));
}













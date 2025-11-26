#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils/evaluate.h"
#include "utils/parameterize.h"

using namespace e2;

class ParameterizeTest : public ::testing::Test {
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

TEST_F(ParameterizeTest, ParameterizePointOnRay) {
    double t = 5.0;
    Vec3d point = evaluatePoint(ray, t);
    double param = parameterize(ray, point);
    EXPECT_DOUBLE_EQ(param, t);
};

TEST_F(ParameterizeTest, ParameterizePointOnCircle) {
    Vec3d center(0, 0, 0);
    Vec3d xaxis(1, 0, 0);
    Vec3d yaxis(0, 1, 0);
    Vec3d normal = xaxis.cross(yaxis);
    double radius = 2.0;
    Cir3d circle(center, radius, normal, xaxis);
    
    // Test point at angle 0 (on positive x-axis)
    Vec3d point1 = center + radius * xaxis;
    double param1 = parameterize(circle, point1);
    EXPECT_NEAR(param1, 0.0, 1e-6);
    
    // Test point at angle π/2 (on positive y-axis)
    Vec3d point2 = center + radius * yaxis;
    double param2 = parameterize(circle, point2);
    EXPECT_NEAR(param2, M_PI/2, 1e-6);
    
    // Test point at angle π (on negative x-axis)
    Vec3d point3 = center - radius * xaxis;
    double param3 = parameterize(circle, point3);
    EXPECT_NEAR(param3, M_PI, 1e-6);
    
    // Test point at angle -π/2 (on negative y-axis)
    Vec3d point4 = center - radius * yaxis;
    double param4 = parameterize(circle, point4);
    EXPECT_NEAR(param4, -M_PI/2, 1e-6);
}

TEST_F(ParameterizeTest, PullIntoRangeFunction) {
    // Test value already in range
    EXPECT_DOUBLE_EQ(pullIntoRange(1.5, 0.0, 2.0), 1.5);
    
    // Test value below range
    EXPECT_NEAR(pullIntoRange(-0.5, 0.0, 2.0), 1.5, 1e-6);
    
    // Test value above range
    EXPECT_NEAR(pullIntoRange(2.5, 0.0, 2.0), 0.5, 1e-6);
    
    // Test value multiple periods away
    EXPECT_NEAR(pullIntoRange(7.0, 1.0, 2.0), 1.0, 1e-6);
    EXPECT_NEAR(pullIntoRange(-3.0, 1.0, 2.0), 1.0, 1e-6);
}

TEST_F(ParameterizeTest, EvaluateRayPoint) {
    double t = 5.0;
    Vec3d point = evaluatePoint(ray, t);
    EXPECT_DOUBLE_EQ(point.x(), 1);
    EXPECT_DOUBLE_EQ(point.y(), 2);
    EXPECT_DOUBLE_EQ(point.z(), 8);
};

TEST_F(ParameterizeTest, EvaluateRayDerivs) {
    double t = 5.0;
    EvaluationResult result = evaluatePointAndDerivs(ray, t, 2);
    EXPECT_EQ(result.nderivsOut, 2);
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[0], Vec3d(1,2,8)));
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[1], Vec3d(0,0,1)));
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[2], Vec3d(0,0,0)));
};

TEST_F(ParameterizeTest, EvaluateRayExcessDerivs) {
    double t = 5.0;
    EvaluationResult result = evaluatePointAndDerivs(ray, t, 5); // request more than 3 derivatives
    EXPECT_EQ(result.nderivsOut, 3); // should only return up to 3
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[0], Vec3d(1,2,8)));
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[1], Vec3d(0,0,1)));
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[2], Vec3d(0,0,0)));
    EXPECT_TRUE(isNearlyEqual(result.derivsOut[3], Vec3d(0,0,0)));
};

TEST_F(ParameterizeTest, EvaluateCirclePoint) {
    // Create a circle with center (1,0,0), radius 2, in XY plane
    Vec3d center(1, 0, 0);
    Vec3d xaxis(1, 0, 0);
    Vec3d yaxis(0, 1, 0);
    Vec3d normal = xaxis.cross(yaxis);
    double radius = 2.0;
    Cir3d circle(center, radius, normal, xaxis);
    
    // Test at t = 0 (should be at (3,0,0))
    Vec3d point = evaluatePoint(circle, 0.0);
    EXPECT_TRUE(isNearlyEqual(point, Vec3d(3, 0, 0)));

    // Test at t = π/2 (should be at (1,2,0))
    point = evaluatePoint(circle, M_PI/2);
    EXPECT_TRUE(isNearlyEqual(point, Vec3d(1, 2, 0)));

    // Test at t = π (should be at (-1,0,0))
    point = evaluatePoint(circle, M_PI);
    EXPECT_TRUE(isNearlyEqual(point, Vec3d(-1, 0, 0)));
}

TEST_F(ParameterizeTest, EvaluateCircleDerivs) {
    Vec3d center(0, 0, 0);
    Vec3d xaxis(1, 0, 0);
    Vec3d yaxis(0, 1, 0);
    Vec3d normal = xaxis.cross(yaxis);
    double radius = 2.0;
    Cir3d circle(center, radius, normal, xaxis);
    
    // Test derivatives at t = 0
    EvaluationResult result = evaluatePointAndDerivs(circle, 0.0, 3);
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












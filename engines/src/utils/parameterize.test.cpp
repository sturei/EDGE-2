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
    Vec3d point = evaluate(ray, t);
    double param = parameterize(ray, point);
    EXPECT_DOUBLE_EQ(param, t);
};

TEST_F(ParameterizeTest, ParameterizeSegmentOnRay) {
    double t1 = 2.0;
    double t2 = 7.0;
    Vec3d start = evaluate(ray, t1);
    Vec3d end = evaluate(ray, t2);
    auto params = parameterize(ray, start, end);
    EXPECT_DOUBLE_EQ(params.first, t1);
    EXPECT_DOUBLE_EQ(params.second, t2);

    // Reverse order. Params are always returned in ascending order
    params = parameterize(ray, end, start);
    EXPECT_DOUBLE_EQ(params.first, t1);
    EXPECT_DOUBLE_EQ(params.second, t2);
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

TEST_F(ParameterizeTest, ParameterizeSegmentOnCircle) {
    Vec3d center(0, 0, 0);
    Vec3d xaxis(1, 0, 0);
    Vec3d yaxis(0, 1, 0);
    Vec3d normal = xaxis.cross(yaxis);
    double radius = 1.0;
    Cir3d circle(center, radius, normal, xaxis);
    
    // Test segment from 0 to π/2
    Vec3d start = center + radius * xaxis;  // angle 0
    Vec3d end = center + radius * yaxis;    // angle π/2
    auto params = parameterize(circle, start, end);
    EXPECT_NEAR(params.first, 0.0, 1e-6);
    EXPECT_NEAR(params.second, M_PI/2, 1e-6);
    
    // Test segment crossing angle 0 (from -π/4 to π/4)
    Vec3d start2 = evaluate(circle, -M_PI/4);
    Vec3d end2 = evaluate(circle, M_PI/4);
    auto params2 = parameterize(circle, start2, end2);
    EXPECT_NEAR(params2.first, -M_PI/4, 1e-6);
    EXPECT_NEAR(params2.second, M_PI/4, 1e-6);

    // Test the complement of the above segment (from π/4 to -π/4)
    Vec3d start3 = evaluate(circle, M_PI/4);
    Vec3d end3 = evaluate(circle, -M_PI/4)  ;
    auto params3 = parameterize(circle, start3, end3);
    EXPECT_NEAR(params3.first, M_PI/4, 1e-6);
    EXPECT_NEAR(params3.second, 7*M_PI/4, 1e-6);

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










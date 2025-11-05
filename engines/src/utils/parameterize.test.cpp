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










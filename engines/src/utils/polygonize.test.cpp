#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils/polygonize.h"

using namespace e2;

class PolygonizeTest : public ::testing::Test {
    protected:
        void SetUp() override {
            ray = Ray3d(Vec3d(1,2,3), Vec3d(0,0,1));
            circle = Cir3d(Vec3d(0,0,0), 1.0, Vec3d(0,0,1), Vec3d(1,0,0));
        };

        void TearDown() override {
        }
        Ray3d ray;
        Cir3d circle;
        bool isNearlyEqual(const Vec3d& v1, const Vec3d& v2, double tol=1e-6) {
            return (std::abs(v1.x() - v2.x()) < tol) &&
                   (std::abs(v1.y() - v2.y()) < tol) &&
                   (std::abs(v1.z() - v2.z()) < tol);
        }   
};

TEST_F(PolygonizeTest, PolygonizeRay) {
    double tstart = 2.0;
    double tend = 7.0;
    auto points = polygonize(ray, tstart, tend);
    ASSERT_EQ(points->size(), 2);
    delete points;
};

TEST_F(PolygonizeTest, PolygonizeFullCircle) {
    double tstart = 0.0;
    double tend = 2.0 * M_PI;
    auto points = polygonize(circle, tstart, tend);
    EXPECT_TRUE(isNearlyEqual(points->front(), points->back()));
    EXPECT_GT(points->size(), 10); // Expect multiple segments
    delete points;
};




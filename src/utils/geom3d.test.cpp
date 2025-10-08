#include "utils/geom3d.h"
#include <gtest/gtest.h>

using namespace e2;

TEST(Geom3dTest, DefaultConstructor) {
    Geom3d g;

    EXPECT_EQ(g.position(), Vec3d(0,0,0));
    EXPECT_EQ(g.direction(), Vec3d(1,0,0));
    EXPECT_EQ(g.direction2(), Vec3d(0,1,0));
    EXPECT_EQ(g.scale(), 1.0);
    EXPECT_EQ(g.scale2(), 1.0);
}   

TEST(Geom3dTest, ParameterizedConstructor) {
    Vec3d pos(1,2,3);
    Vec3d dir(0,1,0);
    Vec3d dir2(0,0,1);
    double scale = 5.0;
    double scale2 = 10.0;

    Geom3d g(pos, dir, dir2, scale, scale2);

    EXPECT_EQ(g.position(), pos);
    EXPECT_EQ(g.direction(), dir);
    EXPECT_EQ(g.direction2(), dir2);
    EXPECT_EQ(g.scale(), scale);
    EXPECT_EQ(g.scale2(), scale2);
}

TEST(Plane3dTest, DefaultConstructor) {
    Plane3d p;

    EXPECT_EQ(p.position(), Vec3d(0,0,0));
    EXPECT_EQ(p.normal(), Vec3d(1,0,0));
}   

TEST(Plane3dTest, ParameterizedConstructor) {
    Vec3d pos(1,2,3);
    Vec3d normal(0,1,0);
    Plane3d p(pos, normal);
    EXPECT_EQ(p.position(), pos);
    EXPECT_EQ(p.normal(), normal);
}   

TEST(Sphere3dTest, DefaultConstructor) {
    Sphere3d s;

    EXPECT_EQ(s.center(), Vec3d(0,0,0));
    EXPECT_EQ(s.radius(), 1.0);
}   

TEST(Sphere3dTest, ParameterizedConstructor) {
    Vec3d center(1,2,3);
    double radius = 5.0;
    Sphere3d s(center, radius);
    EXPECT_EQ(s.center(), center);
    EXPECT_EQ(s.radius(), radius);
}   

TEST(Point3dTest, DefaultConstructor) {
    Point3d p;      
    EXPECT_EQ(p.position(), Vec3d(0,0,0));
}

TEST(Point3dTest, ParameterizedConstructor) {
    Vec3d pos(1,2,3);
    Point3d p(pos);
    EXPECT_EQ(p.position(), pos);
}   

TEST(Line3dTest, DefaultConstructor) {
    Line3d l;      
    EXPECT_EQ(l.position(), Vec3d(0,0,0));
    EXPECT_EQ(l.direction(), Vec3d(1,0,0));
}   

TEST(Line3dTest, ParameterizedConstructor) {
    Vec3d pos(1,2,3);
    Vec3d dir(0,1,0);
    Line3d l(pos, dir);
    EXPECT_EQ(l.position(), pos);
    EXPECT_EQ(l.direction(), dir);
}

TEST(Circle3dTest, DefaultConstructor) {
    Circle3d c;      
    EXPECT_EQ(c.center(), Vec3d(0,0,0));
    EXPECT_EQ(c.normal(), Vec3d(1,0,0));
    EXPECT_EQ(c.radius(), 1.0);
}

TEST(Circle3dTest, ParameterizedConstructor) {
    Vec3d center(1,2,3);
    Vec3d normal(0,1,0);
    double radius = 5.0;
    Circle3d c(center, normal, radius);
    EXPECT_EQ(c.center(), center);
    EXPECT_EQ(c.normal(), normal);
    EXPECT_EQ(c.radius(), radius);
}   

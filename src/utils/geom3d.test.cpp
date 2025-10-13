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

    //printf("sizeof(Geom3d): %zu\n", sizeof(Geom3d));
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

TEST(Geom3dTest, StreamOutputOperator) {
    Vec3d pos(1,2,3);
    Vec3d dir(0,1,0);
    Vec3d dir2(0,0,1);
    double scale = 5.0;
    double scale2 = 10.0;
    Geom3d g(pos, dir, dir2, scale, scale2);

    std::ostringstream oss;
    oss << g;
    std::string geomStr = oss.str();

    //std::cout << "Geom3d output:\n" << geomStr << std::endl; --- IGNORE ---

    // Check that the output string contains some expected substrings
    EXPECT_NE(geomStr.find("pos=(1, 2, 3)"), std::string::npos);
    EXPECT_NE(geomStr.find("dir=(0, 1, 0)"), std::string::npos);
    EXPECT_NE(geomStr.find("dir2=(0, 0, 1)"), std::string::npos);
    EXPECT_NE(geomStr.find("scale=5"), std::string::npos);
    EXPECT_NE(geomStr.find("scale2=10"), std::string::npos);
}

TEST(Plane3dTest, DefaultConstructor) {
    Plane3d p;
    EXPECT_EQ(p.position(), Vec3d(0,0,0));
    //EXPECT_EQ(p.normal(), Vec3d(1,0,0));
}   

TEST(Plane3dTest, ParameterizedConstructor) {
    Vec3d pos(1,2,3);
    Vec3d normal(0,1,0);
    Plane3d p(pos, normal);
    EXPECT_EQ(p.position(), pos);
    EXPECT_EQ(p.normal(), normal);
}

TEST(Plane3dTest, StreamOutputOperator) {
    Vec3d pos(1,2,3);
    Vec3d normal(0,1,0);
    Plane3d p(pos, normal);

    std::ostringstream oss;
    oss << p;
    std::string planeStr = oss.str();

    //std::cout << "Plane3d output:\n" << planeStr << std::endl; --- IGNORE ---

    // Check that the output string contains some expected substrings
    EXPECT_NE(planeStr.find("pos=(1, 2, 3)"), std::string::npos);
    EXPECT_NE(planeStr.find("normal=(0, 1, 0)"), std::string::npos);
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

TEST(Sphere3dTest, StreamOutputOperator) {
    Vec3d center(1,2,3);
    double radius = 5.0;
    Sphere3d s(center, radius);

    std::ostringstream oss;
    oss << s;
    std::string sphereStr = oss.str();

    //std::cout << "Sphere3d output:\n" << sphereStr << std::endl; --- IGNORE ---

    // Check that the output string contains some expected substrings
    EXPECT_NE(sphereStr.find("center=(1, 2, 3)"), std::string::npos);
    EXPECT_NE(sphereStr.find("radius=5"), std::string::npos);
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

TEST(Point3dTest, StreamOutputOperator) {
    Vec3d pos(1,2,3);
    Point3d p(pos);

    std::ostringstream oss;
    oss << p;
    std::string pointStr = oss.str();

    //std::cout << "Point3d output:\n" << pointStr << std::endl; --- IGNORE ---

    // Check that the output string contains some expected substrings
    EXPECT_NE(pointStr.find("pos=(1, 2, 3)"), std::string::npos);
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

TEST(Line3dTest, StreamOutputOperator) {
    Vec3d pos(1,2,3);
    Vec3d dir(0,1,0);
    Line3d l(pos, dir);

    std::ostringstream oss;
    oss << l;
    std::string lineStr = oss.str();

    //std::cout << "Line3d output:\n" << lineStr << std::endl; --- IGNORE ---

    // Check that the output string contains some expected substrings
    EXPECT_NE(lineStr.find("pos=(1, 2, 3)"), std::string::npos);
    EXPECT_NE(lineStr.find("dir=(0, 1, 0)"), std::string::npos);
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

TEST(Circle3dTest, StreamOutputOperator) {
    Vec3d center(1,2,3);
    Vec3d normal(0,1,0);
    double radius = 5.0;
    Circle3d c(center, normal, radius);

    std::ostringstream oss;
    oss << c;
    std::string circleStr = oss.str();

    //std::cout << "Circle3d output:\n" << circleStr << std::endl; --- IGNORE ---

    // Check that the output string contains some expected substrings
    EXPECT_NE(circleStr.find("center=(1, 2, 3)"), std::string::npos);
    EXPECT_NE(circleStr.find("normal=(0, 1, 0)"), std::string::npos);
    EXPECT_NE(circleStr.find("radius=5"), std::string::npos);
}

TEST(Geom3dTest, PackUnpack) {
    
    Vec3d pos(1,2,3);
    Vec3d normal(0,1,0);
    Plane3d plIn(pos, normal);

    Vec3d center(4,5,6);
    double radius = 5.0;
    Sphere3d spIn(center, radius);

    Vec3d location(7,8,9);
    Point3d poIn(location);

    // pack
    std::vector<Geom3d> geoms{ plIn, spIn, poIn };

    // unpack
    const Plane3d& plOut = geomAsPlane(geoms[0]);
    const Sphere3d& spOut = geomAsSphere(geoms[1]);
    const Point3d& poOut = geomAsPoint(geoms[2]);

    EXPECT_EQ(plOut.position(), plIn.position());
    EXPECT_EQ(plOut.normal(), plIn.normal()); 
    EXPECT_EQ(spOut.center(), spIn.center());
    EXPECT_EQ(spOut.radius(), spIn.radius()); 
    EXPECT_EQ(poOut.position(), poIn.position());

}

#include "utils/geom3d.h"
#include <gtest/gtest.h>

using namespace e2;

TEST(Geom3dTest, DefaultConstructor) {
    Geom3d g;
    EXPECT_EQ(g.type(), Geom3dType::RUBBER);
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
    Geom3d g(Geom3dType::RUBBER, pos, dir, dir2, scale, scale2);
    EXPECT_EQ(g.type(), Geom3dType::RUBBER);    
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
    Geom3d g(Geom3dType::RUBBER, pos, dir, dir2, scale, scale2);

    std::ostringstream oss;
    oss << g;
    std::string geomStr = oss.str();

    //std::cout << "Geom3d output:\n" << geomStr << std::endl; --- IGNORE ---

    // Check that the output string contains some expected substrings
    EXPECT_NE(geomStr.find("RUBBER"), std::string::npos);
    EXPECT_NE(geomStr.find("pos=(1, 2, 3)"), std::string::npos);
    EXPECT_NE(geomStr.find("dir=(0, 1, 0)"), std::string::npos);
    EXPECT_NE(geomStr.find("dir2=(0, 0, 1)"), std::string::npos);
    EXPECT_NE(geomStr.find("scale=5"), std::string::npos);
    EXPECT_NE(geomStr.find("scale2=10"), std::string::npos);
}

TEST(Geom3dTest, ConstructorFromPoint) {
    Vec3d pos(1,2,3);
    Geom3d g(pos);
    EXPECT_EQ(g.type(), Geom3dType::POINT);
    EXPECT_EQ(g.position(), pos);
    EXPECT_EQ(g.direction(), Vec3d(1,0,0));
    EXPECT_EQ(g.direction2(), Vec3d(0,1,0));
    EXPECT_EQ(g.scale(), 1.0);
    EXPECT_EQ(g.scale2(), 1.0);
};

TEST(Geom3dTest, ConstructorFromLine) {
    Vec3d pos(1,2,3);
    Vec3d dir(0,1,0);
    Ray3d line(pos, dir);
    Geom3d g(line);
    EXPECT_EQ(g.type(), Geom3dType::LINE);
    EXPECT_EQ(g.position(), pos);
    EXPECT_EQ(g.direction(), dir);
    EXPECT_EQ(g.direction2(), Vec3d(0,1,0));
    EXPECT_EQ(g.scale(), 1.0);
    EXPECT_EQ(g.scale2(), 1.0);
};  

TEST(Geom3dTest, ConstructorFromCircle) {
    Vec3d center(1,2,3);
    Vec3d normal(0,0,1);
    double radius = 3.0;
    Cir3d circle(center, radius, normal);
    Geom3d g(circle);
    EXPECT_EQ(g.type(), Geom3dType::CIRCLE);
    EXPECT_EQ(g.position(), center);
    EXPECT_EQ(g.direction(), normal);
    EXPECT_EQ(g.direction2(), Vec3d(0,1,0));
    EXPECT_EQ(g.scale(), radius);
    EXPECT_EQ(g.scale2(), 1.0);
};  

TEST(Geom3dTest, ConstructorFromPlane) {
    Vec3d pos(1,2,3);
    Vec3d normal(0,1,0);
    Pla3d pla(pos, normal);
    Geom3d g(pla);
    EXPECT_EQ(g.type(), Geom3dType::PLANE);
    EXPECT_EQ(g.position(), pos);
    EXPECT_EQ(g.direction(), normal);
    EXPECT_EQ(g.direction2(), Vec3d(0,1,0));
    EXPECT_EQ(g.scale(), 1.0);
    EXPECT_EQ(g.scale2(), 1.0);
};

TEST(Geom3dTest, ConstructorFromSphere) {
    Vec3d center(1,2,3);
    double radius = 3.0;
    Sph3d sph(center, radius);
    Geom3d g(sph);
    EXPECT_EQ(g.type(), Geom3dType::SPHERE);
    EXPECT_EQ(g.position(), center);
    EXPECT_EQ(g.direction(), Vec3d(1,0,0));
    EXPECT_EQ(g.direction2(), Vec3d(0,1,0));
    EXPECT_EQ(g.scale(), radius);
    EXPECT_EQ(g.scale2(), 1.0);
};


TEST(Geom3dTest, PackUnpack) {
    
    Vec3d pos(1,2,3);
    Vec3d normal(0,1,0);
    Pla3d plIn(pos, normal);

    Vec3d center(4,5,6);
    double radius = 5.0;
    Sph3d spIn(center, radius);

    Vec3d poIn(7,8,9);

    // pack
    std::vector<Geom3d> geoms{ plIn, spIn, poIn };

    // unpack
    Pla3d plOut;
    Sph3d spOut;
    Vec3d poOut;
    bool isPl = geoms[0].isPlane(plOut);
    bool isSp = geoms[1].isSphere(spOut);
    bool isPo = geoms[2].isPoint(poOut);

    EXPECT_TRUE(isPl);
    EXPECT_TRUE(isSp);
    EXPECT_TRUE(isPo);
    EXPECT_EQ(plOut, plIn);
    EXPECT_EQ(spOut, spIn);
    EXPECT_EQ(poOut, poIn);

}

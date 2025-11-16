#pragma once

#include "utils/geom3d.h" 
    
namespace e2 {
    double pullIntoRange(double t, double ts, double period);
    double parameterize(const Ray3d& ray, const Vec3d& p);
    double parameterize(const Cir3d& cir, const Vec3d& p);
    double parameterize(const Geom3d& geom, const Vec3d& p, double rangeStart = 0.0);

    // TOGO?
    std::pair<double, double> parameterize(const Ray3d& ray, const Vec3d& start, const Vec3d& end);
    std::pair<double, double> parameterize(const Cir3d& cir, const Vec3d& start, const Vec3d& end);
    std::pair<double, double> parameterize(const Geom3d& geom, const Vec3d& start, const Vec3d& end);
};

    

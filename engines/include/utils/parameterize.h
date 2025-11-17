#pragma once

#include "utils/geom3d.h" 
    
namespace e2 {
    double pullIntoRange(double t, double ts, double period);
    double parameterize(const Ray3d& ray, const Vec3d& p);
    double parameterize(const Cir3d& cir, const Vec3d& p);
    double parameterize(const Geom3d& geom, const Vec3d& p, double rangeStart = 0.0);
    // TODO bool parameterize(const BoundedCurve& boundedCurve, const Vec3d& p, CVec &cvecOut);

};

    

#pragma once

#include "utils/geom3d.h" 
#include <array>
    
namespace e2 {
    double pullIntoRange(double t, double ts, double period);
    double parameterize(const Ray3d& ray, const Vec3d& p);
    double parameterize(const Cir3d& cir, const Vec3d& p);
    double parameterize(const Geom3d& geom, const Vec3d& p, double rangeStart = 0.0);

    struct EvaluationResult {
        int nderivsOut = 0;                // Highest derivative returned. May be less than requested, but will always return at least the zeroth derivative.
        std::array<Vec3d, 4> derivsOut;    // Evaluated point and derivatives. derivsOut[0] is the point, derivsOut[1] is the first derivative etc.
    };

    Vec3d evaluatePoint(const Ray3d& ray, double t);
    Vec3d evaluatePoint(const Cir3d& cir, double t);
    Vec3d evaluatePoint(const Geom3d& geom, double t);
    
    EvaluationResult evaluatePointAndDerivs(const Ray3d& ray, double t, int nderivs);
    EvaluationResult evaluatePointAndDerivs(const Cir3d& cir, double t, int nderivs);
    EvaluationResult evaluatePointAndDerivs(const Geom3d& geom, double t, int nderivs);   

};

    

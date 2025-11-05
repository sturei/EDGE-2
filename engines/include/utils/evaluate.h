#pragma once

#include "utils/geom3d.h"

namespace e2 {

    struct EvaluationResult {
        int nderivsOut = 0;                // Highest derivative returned. May be less than requested, but will always return at least the zeroth derivative.
        std::array<Vec3d, 4> derivsOut;    // Evaluated point and derivatives. derivsOut[0] is the point, derivsOut[1] is the first derivative etc.
    };

    Vec3d evaluate(const Ray3d& ray, double t);
    Vec3d evaluate(const Cir3d& cir, double t);
    
    EvaluationResult evaluate(const Ray3d& ray, double t, int nderivs);
    EvaluationResult evaluate(const Cir3d& cir, double t, int nderivs);
};


    

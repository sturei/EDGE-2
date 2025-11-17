#pragma once
#include "utils/ray3d.h"
#include "utils/cir3d.h"
#include "utils/geom3d.h"
#include <vector>

namespace e2 {

    std::vector<Vec3d>* polygonize(const Ray3d& ray, double tstart, double tend);
    std::vector<Vec3d>* polygonize(const Cir3d& circle, double tstart, double tend, double atol = 4*M_PI/180);
    std::vector<Vec3d>* polygonizeBoundedCurve(const BoundedCurve& boundedCurve, double atol = 4*M_PI/180);

};

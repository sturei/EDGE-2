#pragma once

#include "utils/geom3d.h"

namespace e2 {

    std::vector<Vec3d> tessellate(const Ray3d& ray, double tstart, double tend);
    std::vector<Vec3d> tessellate(const Cir3d& circle, double tstart, double tend, double atol = 4*M_PI/180);

};

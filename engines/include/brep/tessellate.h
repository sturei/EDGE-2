#pragma once
#include "utils/ray3d.h"
#include "utils/cir3d.h"
#include "brep/body.h"
#include <vector>

namespace e2 {

    std::vector<Vec3d>* tessellate(const Ray3d& ray, double tstart, double tend);
    std::vector<Vec3d>* tessellate(const Cir3d& circle, double tstart, double tend, double atol = 4*M_PI/180);
    std::vector<Vec3d>* tessellate(const Body& body, CellIndex cellIndex, double atol = 4*M_PI/180);

};

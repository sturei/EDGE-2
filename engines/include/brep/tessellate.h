#pragma once
#include "brep/body.h"
#include <vector>

namespace e2 {
    std::vector<Vec3d>* tessellateEdge(CellIndex edgeIndex, const Body& body, double atol = 4*M_PI/180);
};

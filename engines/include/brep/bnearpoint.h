#pragma once
#include "brep/body.h"
#include "utils/vec3d.h"

namespace e2 {  

    bool nearpointOnProfileVertex(CellIndex vertex, const Body& body, const Vec3d& p, Vec3d& pointOut);
    bool nearpointOnProfileEdge(CellIndex edge, const Body& body, const Vec3d& p, Vec3d& pointOut);
    bool nearpointOnProfileCell(CellIndex cell, const Body& body, const Vec3d& p, Vec3d& pointOut);
    bool nearpointOnProfile(const Body& body, const Vec3d& p, Vec3d& pointOut, CellIndex& cellOut);

}  

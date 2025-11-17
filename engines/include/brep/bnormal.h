#pragma once
#include "brep/body.h"
#include "utils/vec3d.h"

namespace e2 {  

    bool pseudoNormalOnProfileVertex(CellIndex vertex, const Body& body, Vec3d& normalOut);
    bool pseudoNormalOnProfileEdge(CellIndex edge, const Body& body, const Vec3d& posOnEdge, Vec3d& normalOut);
    bool pseudoNormalOnProfileCell(CellIndex cell, const Body& body, const Vec3d& posOnFace, Vec3d& normalOut);

}  

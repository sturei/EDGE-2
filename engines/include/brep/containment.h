#pragma once

#include "utils/vec3d.h"
#include "brep/body.h"

namespace e2 {
    bool vertexContainsPosition(CellIndex vertex, const Body& body, const Vec3d& position);
    bool edgeContainsPosition(CellIndex edge, const Body& body, const Vec3d& position);
    bool faceContainsPosition(CellIndex face, const Body& body, const Vec3d& position);
    bool lumpContainsPosition(CellIndex lump, const Body& body, const Vec3d& position);
    bool cellContainsPosition(CellIndex cell, const Body& body, const Vec3d& position);
    bool bodyContainsPosition(const Body& body, const Vec3d& position);
}

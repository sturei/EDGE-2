#pragma once

#include "brep/body.h"
#include "utils/geom3d.h"

namespace e2 {
    
    // Unified methods for navigating brep bodies of any dimension.
    std::vector<CellIndex> getKSkeleton(int k, const Body& body);
    std::vector<std::pair<CellIndex, CocellSense>> getKBoundary(int k, const CellIndex& cell, const Body& body);
    std::vector<std::pair<CellIndex, CocellSense>> getKStar(int k, const CellIndex& cell, const Body& body);

    // Convenience methods for traditional brep navigation tasks
    std::vector<std::pair<CellIndex, CocellSense>> getEdgesOfFace(const CellIndex& face, const Body& body);
    std::vector<CellIndex> getVerticesOfEdge(const CellIndex& edge, const Body& body);

    // Convenience methods for common geometry tasks
    BoundedCurve getBoundedCurveOfEdge(const CellIndex& edge, const Body& body);

};
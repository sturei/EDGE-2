#pragma once

#include "brep/body.h"
#include "utils/geom3d.h"

namespace e2 {
    
    // Unified methods for navigating brep bodies of any dimension.
    std::vector<CellIndex> getKSkeleton(int k, const Body& body, bool includeActiveOnly = true);
    std::vector<std::pair<CellIndex, CocellSense>> getKBoundary(int k, CellIndex cell, const Body& body, bool includeActiveOnly = true, bool includeExternalOnly = false);
    std::vector<std::pair<CellIndex, CocellSense>> getKStar(int k, CellIndex cell, const Body& body, bool includeActiveOnly = true, bool includeExternalOnly = false);

    // Topological utilities
    std::vector<std::pair<CellIndex, CocellSense>> getAdjacentEdgesInFace(CellIndex vertex, CellIndex face, const Body& body);

    // Some utilities for working with profile bodies (2D bodies embedded in 3D space)
    bool getProfileFace(const Body& body, CellIndex& profileFaceOut);
    bool getProfilePlane(CellIndex profileFace, const Body& body, Pla3d& planeOut);
    bool getProfileEdgeSense(CellIndex edge, CellIndex profileFace, const Body& body, CocellSense& senseOut);

    // Geometric utilities
    BoundedCurve getBoundedCurveOfEdge(const CellIndex& edge, const Body& body);

};
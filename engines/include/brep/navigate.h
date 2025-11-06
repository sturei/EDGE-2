#pragma once

#include "brep/body.h"

namespace e2 {
    
    std::vector<CellIndex> getKSkeleton(int k, const Body& body);
    std::vector<std::pair<CellIndex, CocellSense>> getKBoundary(int k, const CellIndex& cell, const Body& body);
    std::vector<std::pair<CellIndex, CocellSense>> getKStar(int k, const CellIndex& cell, const Body& body);

    // Convenience functions for traditional brep navigation tasks
    std::vector<CellIndex> getFacesOfBody(const Body& body);
    std::vector<std::pair<CellIndex, CocellSense>> getEdgesOfFace(const CellIndex& face, const Body& body);
    std::vector<std::pair<CellIndex, CocellSense>> getVerticesOfEdge(const CellIndex& edge, const Body& body);

};
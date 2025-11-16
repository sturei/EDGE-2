#pragma once
#include "brep/body.h"
#include "utils/pla3d.h"

namespace e2 {

    struct SegmentBoundary {
        CVec cvec;
        int sense;  // +1 for positive side of plane, -1 for negative side, 0 for on plane
    };
    typedef std::pair<SegmentBoundary, SegmentBoundary> Segment;
    std::vector<Segment> segmentEdgeByPlane(CellIndex edgeIndex, const Body& body, const Pla3d& plane);
    
}

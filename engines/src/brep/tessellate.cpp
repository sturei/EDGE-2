#include "brep/tessellate.h"
#include "utils/polygonize.h"
#include "brep/navigate.h"

namespace e2 {

    std::vector<Vec3d>* tessellateEdge(CellIndex edgeIndex, const Body& body, double atol)
    {
        // TODO consider caching the tessellation as an attribute of the edge
        BoundedCurve boundedCurve = getBoundedCurveOfEdge(edgeIndex, body);  
        return polygonizeBoundedCurve(boundedCurve, atol);

    }

};

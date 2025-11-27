#include "brep/bnearpoint.h"
#include "brep/body.h"
#include "brep/containment.h" 
#include "brep/navigate.h"
#include "utils/nearpoint.h"

namespace e2 {  

    bool nearpointOnProfileVertex(CellIndex vertex, const Body& body, const Vec3d& p, Vec3d& pointOut) {
        Geom3d geom = body.cell(vertex).support();
        Vec3d point;
        if (!geom.isPoint(point)) {
            std::cerr << "cell is not a vertex" << std::endl; 
            return false;
        }
        pointOut = point;
        return true;
    }

    bool nearpointOnProfileEdge(CellIndex edge, const Body& body, const Vec3d& p, Vec3d& pointOut) {
        BoundedCurve boundedCurve = getBoundedCurveOfEdge(edge, body);
        return nearpoint(boundedCurve, p, pointOut);
    }

    bool nearpointOnProfileCell(CellIndex cell, const Body& body, const Vec3d& p, Vec3d& pointOut) {
        int dimensionality = body.cell(cell).support().dimensionality();
        if (dimensionality == 0) {
            return nearpointOnProfileVertex(cell, body, p, pointOut);
        }
        else if (dimensionality == 1) {
            return nearpointOnProfileEdge(cell, body, p, pointOut);
        }
        else {
            std::cerr << "unsupported dimensionality" << std::endl;
            return false;
        }
    }

    bool nearpointOnProfile(const Body& body, const Vec3d& p, Vec3d& pointOut, CellIndex& cellOut) {
        // check vertices, then edges.
        bool found = false;
        int maxDimensionality = 1;
        for (int dim = 0; dim <= maxDimensionality; ++dim) {
            auto cells = getKSkeleton(dim, body);
            double minDistSq = std::numeric_limits<double>::max();
            for (const auto& cellIndex : cells) {
                Vec3d pointOnCell;
                if (nearpointOnProfileCell(cellIndex, body, p, pointOnCell)) {
                    double distSqVal = distSq(p, pointOnCell);
                    if (distSqVal < minDistSq) {
                        minDistSq = distSqVal;
                        pointOut = pointOnCell;
                        cellOut = cellIndex;
                        found = true;
                    }
                }
            }
        }
        return found;
    }
}


#include "brep/containment.h"
#include "brep/navigate.h"
#include "utils/parameterize.h"
#include "utils/vec3d.h"
#include "utils/evaluate.h"

namespace e2 {
    bool vertexContainsPosition(CellIndex vertexIndex, const Body& body, const Vec3d& pos) {
        Vec3d vertexPos;
        if (body.cell(vertexIndex).support().isPoint(vertexPos)) {
            return positionsEqual(vertexPos, pos);
        }
        else {
            std::cerr << "Cell #" << vertexIndex << " is not a valid vertex." << std::endl;
        }
        return false;
    }
    
    bool edgeContainsPosition(CellIndex edge, const Body& body, const Vec3d& position) {
        BoundedCurve boundedCurve = getBoundedCurveOfEdge(edge, body);
        double t = parameterize(boundedCurve.curve(), position, boundedCurve.start().t());
        Vec3d pointOnCurve = evaluate(boundedCurve.curve(), t);

        if (!positionsEqual(pointOnCurve, position)) {
            return false;
        }

        if (!(boundedCurve.start().t() < t && t < boundedCurve.end().t())) {   
            return false;
        }

        return true;
    }

    // TODO...
    bool faceContainsPosition(CellIndex face, const Body& body, const Vec3d& position);
    bool lumpContainsPosition(CellIndex lump, const Body& body, const Vec3d& position);
    bool cellContainsPosition(CellIndex cell, const Body& body, const Vec3d& position);
    bool bodyContainsPosition(const Body& body, const Vec3d& position);
}

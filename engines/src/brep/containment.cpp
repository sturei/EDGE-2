
#include "brep/containment.h"
#include "brep/navigate.h"
#include "utils/segment.h"
#include "utils/parameterize.h"
#include "utils/vec3d.h"

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
        Vec3d pointOnCurve = evaluatePoint(boundedCurve.curve(), t);
        if (!positionsEqual(pointOnCurve, position)) {
            return false;
        }
        if (!(boundedCurve.start().t() < t && t < boundedCurve.end().t())) {   
            return false;
        }
        return true;
    }

    bool faceContainsPosition(CellIndex face, const Body& body, const Vec3d& position){
        Pla3d plane;
        if (body.cell(face).support().isPlane(plane)) {
            Vec3d xAxis = plane.normal().ortho();
            Vec3d yAxis = plane.normal().cross(xAxis);
            Pla3d orthoPlane = Pla3d(position, yAxis);
            Ray3d line(position, xAxis);
            int numCrossings = 0;     // number of times the positive half-line crosses the face boundaety
            std::vector<std::pair<CellIndex, CocellSense>> edges = getKBoundary(1, face, body);
            for (const std::pair<CellIndex, CocellSense>& edge : edges) {
                std::vector<Segment> segments = segmentEdgeByPlane(edge.first, body, orthoPlane);
                for (const Segment& segment : segments) {
                    if (segment.first.sense == 1 && segment.second.sense == -1) {
                        // found a segment that crosses the orthogonal plane, so the segmentation must have missed an intersection
                        std::cerr << "segment crosses plane unexpectedly" << std::endl;
                    }
                    else if ((segment.first.sense + segment.second.sense) > 0) {
                        // potential crossings found with plane perturbed along its normal. 
                        // accumulate any crossings on the positive half-line from the test point
                        if (segment.first.sense == 0) {
                            double crossingParam = parameterize(line, segment.first.cvec.position());
                            if (crossingParam > 0) {
                                numCrossings++;
                            }
                        }
                        if (segment.second.sense == 0) {
                            double crossingParam = parameterize(line, segment.second.cvec.position());
                            if (crossingParam > 0) {
                                numCrossings++;
                            }
                        }
                    }
                }
            }

            // point is inside the face if the number of crossings is odd
            return (numCrossings % 2) == 1;
        }  
        return false;          
    }

    // TODO...
    bool cellContainsPosition(CellIndex cell, const Body& body, const Vec3d& position);
    bool bodyContainsPosition(const Body& body, const Vec3d& position);
}

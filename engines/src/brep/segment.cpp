#include "utils/vec3d.h"
#include "brep/body.h"  
#include "brep/navigate.h"
#include "utils/parameterize.h"
#include "utils/evaluate.h"
#include "utils/pla3d.h"
#include "utils/intersect.h"

namespace e2 {

    struct SegmentBoundary {
        CVec cvec;
        int sense;  // +1 for positive side of plane, -1 for negative side, 0 for on plane
    };

    typedef std::pair<SegmentBoundary, SegmentBoundary> Segment;

    int signOfDistanceToPlane(const Vec3d& point, const Pla3d& plane) {
        double dist = (point - plane.position()).dot(plane.normal()); 
        if (dist > RESABS) {
            return 1;
        } else if (dist < -RESABS) {
            return -1;
        } else {
            return 0;
        }
    }

    // Computes the segmentation of an edge against a plane.
    std::vector<Segment> segmentEdgeByPlane(CellIndex edgeIndex, const Body& body, const Pla3d& plane) {
        BoundedCurve boundedCurve = getBoundedCurveOfEdge(edgeIndex, body);
        int startSense = signOfDistanceToPlane(boundedCurve.start().position(), plane);
        int endSense = signOfDistanceToPlane(boundedCurve.end().position(), plane);
        
        // compute all segment boundaries by intersecting the bounded curve with the plane
        std::vector<SegmentBoundary> segmentBoundaries;
        std::vector<CVec> intersectionPoints;
        segmentBoundaries.push_back({boundedCurve.start(), startSense});
        if (intersectBoundedCurveWithPlane(boundedCurve, plane, intersectionPoints)) {    
            // process interior intersection points
            for (const CVec& ip : intersectionPoints) {
                segmentBoundaries.push_back({ip, 0});
            }
        }
        segmentBoundaries.push_back({boundedCurve.end(), endSense});

        // create segments between the boundaries.
        std::vector<Segment> segments;
        for (size_t i = 0; i < segmentBoundaries.size() - 1; ++i) {
            const SegmentBoundary& sbStart = segmentBoundaries[i];
            const SegmentBoundary& sbEnd = segmentBoundaries[i + 1];
            if (sbStart.sense == 0 && sbEnd.sense == 0) {
                // both of the boundaries are on the plane 
                // try to find an interior point that is not on the plane, and if so, split the segment there
                // use the mid-point for simplicity (approximation to extreme point)
                double tMid = 0.5 * (sbStart.cvec.t() + sbEnd.cvec.t());
                Vec3d midPos = evaluatePoint(boundedCurve.curve(), tMid);
                int midSense = signOfDistanceToPlane(midPos, plane);
                if (midSense != 0) {
                    CVec midCVec = CVec(midPos, tMid);
                    segments.push_back({sbStart, {midCVec, midSense}});
                    segments.push_back({{midCVec, midSense}, sbEnd});
                }
                else {
                    // entire segment is on the plane
                    segments.push_back({sbStart, sbEnd});
                };
            }
            else {
                // at least one of the endpoints is not on the plane
                segments.push_back({sbStart, sbEnd});
            }
        }
        return segments;
    }
}



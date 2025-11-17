#include "brep/bnearpoint.h"
#include "brep/body.h"
#include "brep/containment.h" 
#include "brep/navigate.h"
#include "utils/nearpoint.h"
#include "utils/evaluate.h"

namespace e2 {  

    bool pseudoNormalOnProfileVertex(CellIndex vertex, const Body& body, Vec3d& normalOut) {
        Geom3d geom = body.cell(vertex).support();
        Vec3d point;
        if (!geom.isPoint(point)) {
            std::cerr << "cell is not a vertex" << std::endl; 
            return false;
        }

        CellIndex profileFace;
        if (!getProfileFace(body, profileFace)) {
            return false;
        }

        Pla3d profilePlane;
        if (!getProfilePlane(profileFace, body, profilePlane)) {
            return false;
        }

        // compute the pseudo-normal at the vertex by averaging the normals of adjacent edges in the profile
        normalOut = Vec3d(0,0,0);
        auto adjacentEdges = getAdjacentEdgesInFace(vertex, profileFace, body);
        for (const auto& edgeSensePair : adjacentEdges) {
            CellIndex edgeIndex = edgeSensePair.first;
            Geom3d edgeGeom = body.cell(edgeIndex).support();
            double t = parameterize(edgeGeom, point);
            Vec3d edgeTangent = evaluatePointAndDerivs(edgeGeom, t, 1).derivsOut[1].normalize();
            Vec3d edgeNormal = edgeSensePair.second * edgeTangent.cross(profilePlane.normal());
            normalOut = normalOut + edgeNormal;
        }

        if (normalOut.magnitudeSquared() > RESNOR_SQ) {
            normalOut = normalOut.normalize();
        } 
        else {
            std::cerr << "could not compute pseudo-normal at vertex" << std::endl;
            normalOut = profilePlane.normal().ortho();
            return false;
        }   

        return true;
    }

    bool pseudoNormalOnProfileEdge(CellIndex edge, const Body& body, const Vec3d& posOnEdge, Vec3d& normalOut) {
        BoundedCurve boundedCurve = getBoundedCurveOfEdge(edge, body);
        double t = parameterize(boundedCurve.curve(), posOnEdge);
        Vec3d edgeTangent = evaluatePointAndDerivs(boundedCurve.curve(), t, 1).derivsOut[1].normalize();

        CellIndex profileFace;
        if (!getProfileFace(body, profileFace)) {
            return false;
        }

        Pla3d profilePlane;
        if (!getProfilePlane(profileFace, body, profilePlane)) {
            return false;
        }

        CocellSense sense;
        if (!getProfileEdgeSense(edge, profileFace, body, sense)) {
            return false;
        }

        normalOut = sense * edgeTangent.cross(profilePlane.normal());
        return true;
    }

    bool pseudoNormalOnProfileCell(CellIndex cell, const Body& body, const Vec3d& posOnFace, Vec3d& normalOut) {
        int dimensionality = body.cell(cell).support().dimensionality();
        if (dimensionality == 0) {
            return pseudoNormalOnProfileVertex(cell, body, normalOut);
        }
        else if (dimensionality == 1) {
            return pseudoNormalOnProfileEdge(cell, body, posOnFace, normalOut);
        }
        else {
            std::cerr << "unsupported dimensionality" << std::endl;
            return false;
        }
    }

}

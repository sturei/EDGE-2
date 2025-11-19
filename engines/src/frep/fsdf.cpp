    
#include "utils/vec3d.h"
#include "frep/fsdf.h"    
#include "brep/bnearpoint.h"
#include "brep/navigate.h"
#include "utils/nearpoint.h"
#include "brep/bnormal.h"
#include <iostream> 

namespace e2 {

    // Evaluates the signed distance function to the profile defined by the Body, in the 2-dimensional plane of the profile.
    bool FProfileSDF::evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        
        CellIndex profileFace;
        Pla3d profilePlane;
        Vec3d nearestPoint;
        CellIndex nearestCell;
        Vec3d normalAtNearest;

        // implementation note: it would probably be more sensible to make a rule that the profile is always in the XY plane

        if (!getProfileFace(m_Body, profileFace)) {
            return false;
        }

        if (!getProfilePlane(profileFace, m_Body, profilePlane)) {
            return false;
        }

        Vec3d positionOnPlane = nearpoint(profilePlane, positionIn);

        if (!nearpointOnProfile(m_Body, positionOnPlane, nearestPoint, nearestCell)) {
            return false;
        }

        if (!pseudoNormalOnProfileCell(nearestCell, m_Body, nearestPoint, normalAtNearest)) {
            return false;
        }

        double sdfValue = dist(positionOnPlane, nearestPoint);
        int sdfSign = (positionOnPlane - nearestPoint).dot(normalAtNearest) < 0 ? -1 : 1;
        valueOut = sdfSign * sdfValue;
        std::cerr << "FProfileSDF::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FProfileSDF::print(std::ostream& os) const {
        os << "FProfileSDF(Body with " << m_Body.numCells() << " cells)";
    }

    // Evaluates the signed distance function to the profile defined by the Body, in the 2-dimensional plane of the profile.
    bool FExtrusionSDF::evaluate(const Vec3d& positionIn, const std::vector<double>& argsIn, double& valueOut) const {
        if (argsIn.size() != 1) {
            return false;
        }

        // see https://iquilezles.org/articles/distfunctions/ for derivation
        // Note: this implementation assumes that the profile in in the XY plane, extrusion of +/- m_depth/2 along Z.

        double profileSDF = argsIn[0];
        double d = profileSDF;
        double w_x = d;
        double w_y = std::abs(positionIn.z()) - m_Depth/2;
        double w_max = std::max(w_x, w_y);
        double w_x_pos = std::max(w_x, 0.0);
        double w_y_pos = std::max(w_y, 0.0);
        double w_length = std::sqrt(w_x_pos * w_x_pos + w_y_pos * w_y_pos);

        valueOut = std::min(w_max, 0.0) + w_length;
        std::cerr << "FExtrusionSDF::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FExtrusionSDF::print(std::ostream& os) const {
        os << "FExtrusionSDF(Depth: " << m_Depth << ")";
    }



}
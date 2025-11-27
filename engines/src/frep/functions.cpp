    
#include "frep/fobject.h"
#include "utils/vec3d.h"
#include "frep/functions.h"    
#include "brep/bnearpoint.h"
#include "brep/navigate.h"
#include "utils/nearpoint.h"
#include "brep/bnormal.h"

#include <iostream> 

namespace e2 {

    // Evaluates the max of its input arguments
    bool FMax::evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const {
        if (argsIn.empty()) {
            return false;
        }
        valueOut = *std::max_element(argsIn.begin(), argsIn.end());
        //std::cerr << "FMax::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FMax::print(std::ostream& os) const {
        os << "FMax";
    }

    // Evaluates the min of its input arguments
    bool FMin::evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const {
        if (argsIn.empty()) {
            return false;
        }
        valueOut = *std::min_element(argsIn.begin(), argsIn.end());
        //std::cerr << "FMin::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FMin::print(std::ostream& os) const {
        os << "FMin";
    }

    // Evaluates the negation of its input argument
    bool FNegation::evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const {
        if (argsIn.size() != 1) {
            return false;
        }
        valueOut = -argsIn[0];
        //std::cerr << "FNegation::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FNegation::print(std::ostream& os) const {
        os << "FNegation";
    }

    // Evaluates to a constant value
    bool FConstant::evaluate(const Vec3d& _positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        valueOut = m_value;
        //std::cerr << "FConstant::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }
    
    void FConstant::print(std::ostream& os) const {
        os << "FConstant(" << m_value << ")";
    }

    // Evaluates to the distance to a plane
    bool FHalfSpace::evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        valueOut = (positionIn - m_plane.position()).dot(m_plane.normal());
        //std::cerr << "FHalfSpace::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FHalfSpace::print(std::ostream& os) const {
        os << "FHalfSpace(Plane: " << m_plane << ")";
    }

    // Evaluates to the value of the wrapped object
    bool FFObject::evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        bool result = m_fobject.evaluate(positionIn, valueOut);
        //std::cerr << "FFObject::evaluate: valueOut = " << valueOut << std::endl;
        return result;
    }

    void FFObject::print(std::ostream& os) const {
        os << "FFObject wrapping FObject";
    }

        // Evaluates the signed distance function to the profile defined by the Body, in the 2-dimensional plane of the profile.
    bool FProfileSDF::evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        
        CellIndex profileFace;
        Pla3d profilePlane;
        Vec3d nearestPoint;
        CellIndex nearestCell;
        Vec3d normalAtNearest;

        // implementation note: it would probably be more sensible to make a rule that the profile is always in the XY plane

        if (!getProfileFace(m_profile, profileFace)) {
            return false;
        }

        if (!getProfilePlane(profileFace, m_profile, profilePlane)) {
            return false;
        }

        Vec3d positionOnPlane = nearpoint(profilePlane, positionIn);

        if (!nearpointOnProfile(m_profile, positionOnPlane, nearestPoint, nearestCell)) {
            return false;
        }

        if (!pseudoNormalOnProfileCell(nearestCell, m_profile, nearestPoint, normalAtNearest)) {
            return false;
        }

        double sdfValue = dist(positionOnPlane, nearestPoint);
        int sdfSign = (positionOnPlane - nearestPoint).dot(normalAtNearest) < 0 ? -1 : 1;
        valueOut = sdfSign * sdfValue;
        //std::cerr << "FProfileSDF::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FProfileSDF::print(std::ostream& os) const {
        os << "FProfileSDF(Body with " << m_profile.numCells() << " cells)";
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
        //std::cerr << "FExtrusionSDF::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FExtrusionSDF::print(std::ostream& os) const {
        os << "FExtrusionSDF(Depth: " << m_Depth << ")";
    }


}
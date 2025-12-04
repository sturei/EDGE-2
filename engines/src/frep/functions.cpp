    
#include "frep/fobject.h"
#include "utils/vec3d.h"
#include "frep/functions.h"    
#include "brep/bnearpoint.h"
#include "brep/navigate.h"
#include "utils/nearpoint.h"
#include "brep/bnormal.h"

#include <iostream> 

namespace e2 {

    //
    // Sphere
    // 

    // Constructor
    FSphere::FSphere(const Sph3d& sphere) : m_sphere(sphere) {}

    // Evaluates to the distance to a sphere
    bool FSphere::evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        valueOut = dist(positionIn, m_sphere.center()) - m_sphere.radius();
        //std::cerr << "FSphere::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FSphere::print(std::ostream& os) const {
        os << "FSphere(Sphere: " << m_sphere << ")";
    }

    //
    // Block
    //

    // Constructor
    FBlock::FBlock(const double width, const double height, const double depth) : m_width(width), m_height(height), m_depth(depth) {}

    // Evaluates to the distance to a block
    bool FBlock::evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        Vec3d dimensions(m_width / 2.0, m_height / 2.0, m_depth / 2.0);
        Vec3d d = Vec3d(std::abs(positionIn.x()), std::abs(positionIn.y()), std::abs(positionIn.z())) - dimensions;
        Vec3d d_pos = Vec3d(std::max(d.x(), 0.0), std::max(d.y(), 0.0), std::max(d.z(), 0.0));
        valueOut = d_pos.magnitude() + std::min(std::max(d.x(), std::max(d.y(), d.z())), 0.0);
        //std::cerr << "FBlock::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }
    
    void FBlock::print(std::ostream& os) const {
        os << "FBlock(Width: " << m_width << ", Height: " << m_height << ", Depth: " << m_depth << ")";
    }

    //
    // Profile
    //

    // Constructor
    FProfileSDF::FProfileSDF(const Body& profile) : m_profile(profile) {}

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

    //
    // Extrusion
    //

    // Constructor
    FExtrusionSDF::FExtrusionSDF(double depth) : m_Depth(depth) {}

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

    //
    // HalfSpace
    //

    // Constructor
    FHalfSpace::FHalfSpace(const Pla3d& plane) : m_plane(plane) {}

    // Evaluates to the distance to a plane
    bool FHalfSpace::evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        valueOut = (positionIn - m_plane.position()).dot(m_plane.normal());
        //std::cerr << "FHalfSpace::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FHalfSpace::print(std::ostream& os) const {
        os << "FHalfSpace(Plane: " << m_plane << ")";
    }

    //
    // Min / Union
    //

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

    //
    // Max / Intersection
    //

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

    //
    // Negation / Complement
    //

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

    //
    // Object encapsulated as a function
    //

    // Constructor
    FFObject::FFObject(const FObject& fobject) : m_fobject(fobject) {}

    // Evaluates to the value of the wrapped object
    bool FFObject::evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        bool result = m_fobject.evaluate(positionIn, valueOut);
        //std::cerr << "FFObject::evaluate: valueOut = " << valueOut << std::endl;
        return result;
    }

    void FFObject::print(std::ostream& os) const {
        os << "FFObject wrapping FObject";
    }

    //
    // Constant
    //

    // Constructor
    FConstant::FConstant(double value) : m_value(value) {}

    // Evaluates to a constant value
    bool FConstant::evaluate(const Vec3d& _positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        valueOut = m_value;
        //std::cerr << "FConstant::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }
    
    void FConstant::print(std::ostream& os) const {
        os << "FConstant(" << m_value << ")";
    }


}
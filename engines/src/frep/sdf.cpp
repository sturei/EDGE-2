    
#include "utils/vec3d.h"
#include "frep/sdf.h"    
#include "brep/bnearpoint.h"
#include "brep/bnormal.h"
#include <iostream> 

namespace e2 {

    // Evaluates the signed distance function to the profile defined by the Body, in the 2-dimensional plane of the profile.
    // The input position is assumed to lie in the plane of the profile.
    bool SDFToProfile::evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        Vec3d nearestPoint;
        CellIndex nearestCell;
        Vec3d normalAtNearest;
        if (!nearpointOnProfile(m_Body, positionIn, nearestPoint, nearestCell)) {
            return false;
        }
        if (!pseudoNormalOnProfileCell(nearestCell, m_Body, nearestPoint, normalAtNearest)) {
            return false;
        }

        double sdfValue = dist(positionIn, nearestPoint);
        int sdfSign = (positionIn - nearestPoint).dot(normalAtNearest) < 0 ? -1 : 1;
        valueOut = sdfSign * sdfValue;
        return true;
    }

    void SDFToProfile::print(std::ostream& os) const {
        os << "SDFToProfile";
    }

}
/**
 * Vec3d represents a 3D vector assumed to be a point or direction in 3D space.
 * Implementation notes:
 * All methods honour value semantics.
 * Operator== uses exact equality of components. Fuzzy equality will be implemented later as a non-member function.
*/

#include <iostream>
#include "utils/vec3d.h"

namespace e2 {

    /** Returns a unit vector orthogonal to this vector
     */
    Vec3d Vec3d::ortho() const {
        double t0 = std::abs(m_x);
        double t1 = std::abs(m_y);
        double t2 = std::abs(m_z);
        
        int imin = 0;
        if (t1 < t0) 
            imin = t2 < t1 ? 2 : 1;
        else 
            imin = t2 < t0 ? 2 : 0;
        
        Vec3d result;
        
        switch (imin)
        {
            case 0:
                result = Vec3d(0.0, m_z, -m_y);
                break;
            case 1:
                result = Vec3d(m_z, 0.0, -m_x);
                break;
            case 2:
                result = Vec3d(m_y, -m_x, 0.0);
                break;
        }
        
        return result.normalize();
    }

    std::ostream& operator<<(std::ostream& os, const Vec3d& v) {
        os << "(" << v.m_x << ", " << v.m_y << ", " << v.m_z << ")";
        return os;
    }
        
};


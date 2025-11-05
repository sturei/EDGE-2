
/** Evaluates a point and up to 3 derivatives at the specified parameter(s)
*/

#include "utils/geom3d.h" 
    
namespace e2 {

    struct EvaluationResult {
        int nderivsOut = 0;                // Highest derivative returned. May be less than requested, but will always return at least the zeroth derivative.
        std::array<Vec3d, 4> derivsOut;    // Evaluated point and derivatives. derivsOut[0] is the point, derivsOut[1] is the first derivative etc.
    };

    inline static Vec3d evaluate(const Ray3d& ray, double t) {
        //  The parametric form of a ray is
        //	R(t) = P + t*D
        return ray.position() + ray.direction() * t;        
    }     
    
    inline static EvaluationResult evaluate(const Ray3d& ray, double t, int nderivs) {
        EvaluationResult result;
        nderivs = std::min(nderivs, 3);
        result.derivsOut[0] = evaluate(ray, t);
        if (nderivs >= 1) 
            result.derivsOut[++result.nderivsOut] = ray.direction();
        for (int i = 1; i < nderivs; ++i) {
            result.derivsOut[++result.nderivsOut] = Vec3d(0,0,0); // higher derivatives are zero for a ray
        }
        return result;
    }   
};


    

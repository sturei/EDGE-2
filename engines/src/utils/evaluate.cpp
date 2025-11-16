
/** Evaluates a point and up to 3 derivatives at the specified parameter(s)
*/

#include "utils/geom3d.h" 
#include "utils/evaluate.h"
    
namespace e2 {

    Vec3d evaluatePoint(const Ray3d& ray, double t) {
        //  The parametric form of a ray is
        //	R(t) = P + t*D
        return ray.position() + ray.direction() * t;        
    }     

    Vec3d evaluatePoint(const Cir3d& cir, double t) {
        // The parametric form for the circle is 
        // R(t)= C + r*cos(t)*X + r*sin(t)*Y
        Vec3d X = cir.xaxis();
        Vec3d Y = cir.yaxis();
        double rcost = cir.radius() * cos(t);
        double rsint = cir.radius() * sin(t);
        return cir.center().addScaled(X, rcost).addScaled(Y, rsint);
    }

    Vec3d evaluatePoint(const Geom3d& geom, double t) {
        Ray3d line;
        Cir3d circle;
        if (geom.isLine(line)) {
            return evaluatePoint(line, t);
        } else if (geom.isCircle(circle)) {
            return evaluatePoint(circle, t);
        } else {
            std::cerr << "unsupported geometry" << std::endl;
            return Vec3d(0,0,0);
        }
    };

    EvaluationResult evaluatePointAndDerivs(const Ray3d& ray, double t, int nderivs) {
        EvaluationResult result;
        nderivs = std::min(nderivs, 3);
        result.derivsOut[0] = evaluatePoint(ray, t);
        if (nderivs >= 1) {
            result.derivsOut[++result.nderivsOut] = ray.direction();
        }
        for (int i = 1; i < nderivs; ++i) {
            result.derivsOut[++result.nderivsOut] = Vec3d(0,0,0); // higher derivatives are zero for a ray
        }
        return result;
    }   

    EvaluationResult evaluatePointAndDerivs(const Cir3d& cir, double t, int nderivs) {
        EvaluationResult result;
        nderivs = std::min(nderivs, 3);
        Vec3d X = cir.xaxis();
        Vec3d Y = cir.yaxis();
        double rcost = cir.radius() * cos(t);
        double rsint = cir.radius() * sin(t);
        result.derivsOut[0] = cir.center().addScaled(X, rcost).addScaled(Y, rsint);
        if (nderivs >= 1) {
            result.derivsOut[++result.nderivsOut] = -rsint*X + rcost*Y;
        }
        if (nderivs >= 2) {
            result.derivsOut[++result.nderivsOut] = -rcost*X - rsint*Y;
        }
        if (nderivs >= 3) {
            result.derivsOut[++result.nderivsOut] = rsint*X - rcost*Y;
        } 
        return result;
    }   

    EvaluationResult evaluatePointAndDerivs(const Geom3d& geom, double t, int nderivs) {
        Ray3d line;
        Cir3d circle;
        if (geom.isLine(line)) {
            return evaluatePointAndDerivs(line, t, nderivs);
        } else if (geom.isCircle(circle)) {
            return evaluatePointAndDerivs(circle, t, nderivs);
        } else {
            std::cerr << "unsupported geometry" << std::endl;
            return EvaluationResult();
        }
    };

};


    

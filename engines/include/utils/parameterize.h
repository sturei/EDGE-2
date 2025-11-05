
/** Projects a point into the parameters space of the geometry
*/

#include "utils/geom3d.h" 
    
namespace e2 {

    inline static double parameterize(const Ray3d& ray, const Vec3d& p) {
        // The parametric form of a ray is
        // R(t) = P + t*D
        // To find t for a point p, we rearrange to get:
        // t = (p - P) . D
        return (p - ray.position()).dot(ray.direction());
    }

    inline static std::pair<double, double> parameterize(const Ray3d& ray, const Vec3d& start, const Vec3d& end) {
        double t1 = parameterize(ray, start);
        double t2 = parameterize(ray, end);
        if (t1 < t2) {
            return std::make_pair(t1, t2);
        } else {
            return std::make_pair(t2, t1);
        }
    }

};

    

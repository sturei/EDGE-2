
/** Projects a point into the parameters space of the geometry
*/

#include "utils/parameterize.h"
    
namespace e2 {

    // pulls a parameter into the range [ts, ts+period)
    double pullIntoRange(double t, double ts, double period) {
        if (ts <= t && t < ts + period) {
            return t;
        }
        double n = std::floor((t - ts) / period);
        t -= n * period;
        return t;
    }

    double parameterize(const Ray3d& ray, const Vec3d& p) {
        return (p - ray.position()).dot(ray.direction());
    }

    double parameterize(const Cir3d& cir, const Vec3d& p) {
        Vec3d diff = p - cir.center();
        double t = std::atan2(diff.dot(cir.yaxis()), diff.dot(cir.xaxis()));
        return t;
    }

    double parameterize(const Geom3d& geom, const Vec3d& p, double rangeStart) {
        Ray3d line;
        Cir3d circle;
        if (geom.isLine(line)) {
            return parameterize(line, p);
        } else if (geom.isCircle(circle)) {
            double t = parameterize(circle, p);
            t = pullIntoRange(t, rangeStart, 2.0 * M_PI);
            return t;
        } else {
            std::cerr << "unsupported geometry" << std::endl;
            return 0.0;
        }
    };   

    std::pair<double, double> parameterize(const Ray3d& ray, const Vec3d& start, const Vec3d& end) {
        double t1 = parameterize(ray, start);
        double t2 = parameterize(ray, end);
        if (t1 < t2) {
            return std::make_pair(t1, t2);
        } else {
            return std::make_pair(t2, t1);
        }
    }

    std::pair<double, double> parameterize(const Cir3d& cir, const Vec3d& start, const Vec3d& end) {
        double t1 = parameterize(cir, start);
        double t2 = parameterize(cir, end);
        t2 = pullIntoRange(t2, t1, 2.0 * M_PI);
        return std::make_pair(t1, t2);
    }

};

    

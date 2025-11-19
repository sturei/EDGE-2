
#include "utils/vec3d.h"
#include "utils/ray3d.h"
#include "utils/cir3d.h"
#include "utils/geom3d.h"
#include "utils/parameterize.h"

namespace e2 {  

    Vec3d nearpoint(const Ray3d& ray, const Vec3d& p) {
        return ray.position() + ray.direction() * (p - ray.position()).dot(ray.direction()); 
    }

    Vec3d nearpoint(const Cir3d& cir, const Vec3d& p) {
        Vec3d diff = p - cir.center();
        Vec3d proj = diff - cir.normal() * diff.dot(cir.normal());
        if (proj.magnitudeSquared() < RESABS_SQ) {
            // point is on the axis of the circle - return point on circle x axis
            return cir.center() + cir.xaxis() * cir.radius();
        }
        return cir.center() + proj.normalize() * cir.radius();
    }

    Vec3d nearpoint(const Pla3d& plane, const Vec3d& p) {
        Vec3d diff = p - plane.position();
        return p - plane.normal() * diff.dot(plane.normal());
    }

    Vec3d nearpoint(const Geom3d& geom, const Vec3d& p) {
        Ray3d line;
        Cir3d circle;
        Pla3d plane;
        if (geom.isLine(line)) {
            return nearpoint(line, p);
        } 
        else if (geom.isCircle(circle)) {
            return nearpoint(circle, p);
        } 
        else if (geom.isPlane(plane)) {
            return nearpoint(plane, p);
        }
        else {
            std::cerr << "unsupported geometry" << std::endl;
            return Vec3d(0,0,0);
        }
    };

    bool nearpoint(const BoundedCurve& boundedCurve, const Vec3d& p, Vec3d& npOut) {
        Geom3d geom = boundedCurve.curve();
        Vec3d np = nearpoint(geom, p);
        double t = parameterize(geom, np, boundedCurve.start().t());
        // check that t is within the bounded curve range
        if (t < boundedCurve.start().t() || t > boundedCurve.end().t()) {
            return false;
        }
        npOut = np;
        return true;
    };

}


    
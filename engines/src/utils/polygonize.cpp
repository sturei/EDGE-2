#include "utils/polygonize.h"
#include "utils/evaluate.h"
#include "brep/navigate.h"
#include "utils/parameterize.h"

namespace e2 {

    std::vector<Vec3d>* polygonize(const Ray3d& ray, double tstart, double tend) {
        std::vector<Vec3d>* points = new std::vector<Vec3d>();
        const Vec3d start = evaluatePoint(ray, tstart);
        const Vec3d end = evaluatePoint(ray, tend);
        points->push_back(start);
        points->push_back(end);
        return points;
    }

    std::vector<Vec3d>* polygonize(const Cir3d& circle, double tstart, double tend, double atol) {
        // Implementation for circle tessellation
        std::vector<Vec3d>* points = new std::vector<Vec3d>();
        double angleSpan = tend - tstart;
        int nSegments = std::max(2, static_cast<int>(std::ceil(angleSpan / atol)));
        double tstep = angleSpan / nSegments;
        points->reserve(nSegments + 1);
        for (int i = 0; i <= nSegments; ++i) {
            double t = tstart + i * tstep;
            points->push_back(evaluatePoint(circle, t));
        }
        return points;
    }

    std::vector<Vec3d>* polygonizeBoundedCurve(const BoundedCurve& boundedCurve, double atol)
    {
        std::vector<Vec3d>* points = new std::vector<Vec3d>();
        double tstart = boundedCurve.start().t();
        double tend = boundedCurve.end().t();
        const Geom3d& curve = boundedCurve.curve();      
        Ray3d line;
        Cir3d circle;
        if (curve.isLine(line)) {
            points = polygonize(line, tstart, tend);
        } else if (curve.isCircle(circle)) {
            points = polygonize(circle, tstart, tend, atol);
        }
        return points;
    }


};

#include "brep/tessellate.h"
#include "utils/evaluate.h"

namespace e2 {

    std::vector<Vec3d> tessellate(const Ray3d& ray, double tstart, double tend) {
        const Vec3d start = evaluate(ray, tstart);
        const Vec3d end = evaluate(ray, tend);
        return {start, end};
    }

    std::vector<Vec3d> tessellate(const Cir3d& circle, double tstart, double tend, double atol) {
        // Implementation for circle tessellation
        std::vector<Vec3d> points;
        double angleSpan = tend - tstart;
        int nSegments = std::max(2, static_cast<int>(std::ceil(angleSpan / atol)));
        double tstep = angleSpan / nSegments;
        points.reserve(nSegments + 1);
        for (int i = 0; i <= nSegments; ++i) {
            double t = tstart + i * tstep;
            points.push_back(evaluate(circle, t));
        }
        return points;
    }

};

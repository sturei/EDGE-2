#include "brep/tessellate.h"
#include "utils/evaluate.h"
#include "brep/navigate.h"
#include "utils/parameterize.h"

namespace e2 {

    // TODO: populate a reference or return a pointer, to avoid copying.
    std::vector<Vec3d>* tessellate(const Ray3d& ray, double tstart, double tend) {
        std::vector<Vec3d>* points = new std::vector<Vec3d>();
        const Vec3d start = evaluate(ray, tstart);
        const Vec3d end = evaluate(ray, tend);
        points->push_back(start);
        points->push_back(end);
        return points;
    }

    std::vector<Vec3d>* tessellate(const Cir3d& circle, double tstart, double tend, double atol) {
        // Implementation for circle tessellation
        std::vector<Vec3d>* points = new std::vector<Vec3d>();
        double angleSpan = tend - tstart;
        int nSegments = std::max(2, static_cast<int>(std::ceil(angleSpan / atol)));
        double tstep = angleSpan / nSegments;
        points->reserve(nSegments + 1);
        for (int i = 0; i <= nSegments; ++i) {
            double t = tstart + i * tstep;
            points->push_back(evaluate(circle, t));
        }
        return points;
    }

    std::vector<Vec3d>* tessellate(const Body& body, CellIndex cellIndex, double atol)
    {
        std::vector<Vec3d>* points = new std::vector<Vec3d>();

        // TODO cache the tessellation as an attribute of the cell

        const Cell& cell = body.cell(cellIndex);
        const Geom3d& support = cell.support();
        if (support.dimensionality() != 1) {
            return points;
        }

        BoundedCurve boundedCurve = getBoundedCurveOfEdge(cellIndex, body);  
        double tstart = boundedCurve.start().t();
        double tend = boundedCurve.end().t();

        Ray3d line;
        Cir3d circle;
        if (support.isLine(line)) {
            points = tessellate(line, tstart, tend);
        } else if (support.isCircle(circle)) {
            points = tessellate(circle, tstart, tend, atol);
        }

        return points;
    }


};

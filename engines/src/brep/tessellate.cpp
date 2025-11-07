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

        Ray3d line;
        Cir3d circle;
        if (support.isLine(line)) {
            std::vector<std::pair<CellIndex, CocellSense>> vertices = getVerticesOfEdge(cellIndex, body);
            if (vertices.size() != 2) {
                return points; // Need exactly two vertices to define the segment
            }
            Vec3d startVertexPos = body.cell(vertices[0].first).support().position();
            Vec3d endVertexPos = body.cell(vertices[1].first).support().position();
            double tstart = parameterize(line, startVertexPos);
            double tend = parameterize(line, endVertexPos);
            points = tessellate(line, tstart, tend);
        } else if (support.isCircle(circle)) {
            double tstart = 0.0;      // Start angle
            double tend = 2.0 * M_PI; // End angle
            std::vector<std::pair<CellIndex, CocellSense>> vertices = getVerticesOfEdge(cellIndex, body);
            if (vertices.size() == 2) {
                Vec3d startVertexPos = body.cell(vertices[0].first).support().position();
                Vec3d endVertexPos = body.cell(vertices[1].first).support().position();
                std::pair<double, double> tRange = parameterize(circle, startVertexPos, endVertexPos);
                tstart = tRange.first;
                tend = tRange.second;
            }
            points = tessellate(circle, tstart, tend, atol);
        }

        return points;
    }


};

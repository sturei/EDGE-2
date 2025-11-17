
#include "brep/body.h"
#include "brep/navigate.h"
#include "utils/evaluate.h"
#include "utils/parameterize.h"

namespace e2 {

    typedef size_t VertexIndex;

    // Returns the k-skeleton of the body (i.e. all cells in the body having dimension k). TODO: check active flag.
    std::vector<CellIndex> getKSkeleton(int k, const Body& body){
        std::vector<CellIndex> result;
        for (CellIndex i = 0; i < body.numCells(); ++i) {
            const Cell& cell = body.cell(i);
            int dimensionality = cell.support().dimensionality();
            if (dimensionality == k) {
                result.push_back(i);
            }
        }
        return result;
    }

    // Returns the k-boundary of the given cell (i.e. all of its boundary cells having dimension k).
    // The boundary cells are returned together with the sense of the boundary wrt the star.
    std::vector<std::pair<CellIndex, CocellSense>> getKBoundary(int k, const CellIndex& cellIndex, const Body& body) {
        std::vector<std::pair<CellIndex, CocellSense>> result;
        const Graph& graph = body.graph();
        const Graph::NodeView vertex = graph.node(cellIndex);
        for (size_t i = 0; i < vertex.outDegree; ++i) {
            Graph::LinkView edge = vertex.outLink(i);
            CocellIndex cocellIndex = edge.linkProperty;
            const Cocell& cocell = body.cocell(cocellIndex);
            const CellIndex boundaryCellIndex = edge.target;
            const Cell& boundaryCell = body.cell(boundaryCellIndex);
            int dimensionality = boundaryCell.support().dimensionality();
            if (dimensionality == k) {
                result.push_back(std::make_pair(boundaryCellIndex, cocell.sense()));    
            }
        }
        return result;
    }

    // Returns the k-star of the given cell (i.e. all of its star cells having dimension k).
    // The star cells are returned together with the sense of the boundary wrt the star
    std::vector<std::pair<CellIndex, CocellSense>> getKStar(int k, const CellIndex& cellIndex, const Body& body) {
        std::vector<std::pair<CellIndex, CocellSense>> result;
        const Graph& graph = body.graph();
        const Graph::NodeView vertex = graph.node(cellIndex);
        for (size_t i = 0; i < vertex.inDegree; ++i) {
            Graph::LinkView edge = vertex.inLink(i);
            CocellIndex cocellIndex = edge.linkProperty;
            const Cocell& cocell = body.cocell(cocellIndex);
            const CellIndex starCellIndex = edge.source;
            const Cell& starCell = body.cell(starCellIndex);
            int dimensionality = starCell.support().dimensionality();
            if (dimensionality == k) {
                result.push_back(std::make_pair(starCellIndex, cocell.sense()));    
            }
        }
        return result;
    }

    // Returns all boundary edges (1-cells) of the given face (2-cell)
    std::vector<std::pair<CellIndex, CocellSense>> getEdgesOfFace(const CellIndex& face, const Body& body) { 
        std::vector<std::pair<CellIndex, CocellSense>> result;
        auto boundaries = getKBoundary(1, face, body); 
        for (const auto& edge : boundaries) {
            if (edge.second != 0) {  
                result.push_back(edge);
            }
        }
        return result;
    }

    // Returns the start and end vertices (0-cells) of the given edge (1-cell), in <st, en> order, or no vertices if the edge is unbounded
    std::vector<CellIndex> getVerticesOfEdge(const CellIndex& edge, const Body& body) {
        std::vector<CellIndex> result;
        auto boundaries = getKBoundary(0, edge, body);

        // TODO: consider filtering for external and active cells in getKBoundary
        std::vector<std::pair<CellIndex, CocellSense>> externalBoundaries;
        for (const auto& vertex : boundaries) {
            if (vertex.second != 0) { 
                externalBoundaries.push_back(vertex);
            }
        }
        if (externalBoundaries.size() == 2 && externalBoundaries[1].second == -1) {
            // flip the order so that we return the vertices in <start, end> order wrt the support direction, as a convenience to the caller
            std::reverse(externalBoundaries.begin(), externalBoundaries.end());
        }
        for (const auto& vertex : externalBoundaries) {
            result.push_back(vertex.first);
        }
        return result;
    }

    // Returns the start and end of the edge as a bounded curve with monotonic parameterization
    BoundedCurve getBoundedCurveOfEdge(const CellIndex& edge, const Body& body) {
        Geom3d curve = body.cell(edge).support();

        const std::vector<CellIndex> vertices = getVerticesOfEdge(edge, body);
        if (vertices.size() != 2) {
            // edge is not bounded by start and end vertices. Return all of its geometry as the bounded curve.
            // TODO: consider carrying a flag in bounded curve to indicate unboundedness
            double periodicity;
            if (curve.isPeriodicCurve(periodicity)) {
                CVec start = { evaluatePoint(curve, 0.0), 0.0 };
                CVec end = { evaluatePoint(curve, periodicity), periodicity };
                return BoundedCurve(curve, start, end);
            } else {
                CVec start = { evaluatePoint(curve, -SIZE), -SIZE };
                CVec end = { evaluatePoint(curve, SIZE), SIZE };
                return BoundedCurve(curve, start, end);
            }   
        }

        Vec3d vStart = body.cell(vertices[0]).support().position();
        Vec3d vEnd = body.cell(vertices[1]).support().position();

        double tStart = parameterize(curve, vStart);
        double tEnd = parameterize(curve, vEnd, tStart);
 
        return BoundedCurve(curve, CVec(vStart, tStart), CVec(vEnd, tEnd));
    }        

};
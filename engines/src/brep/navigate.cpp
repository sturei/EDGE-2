
#include "brep/body.h"

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

    // Returns all faces (2-cells) of the body
    std::vector<CellIndex> getFacesOfBody(const Body& body) { 
        return getKSkeleton(2, body); 
    }

    // Returns all edges (1-cells) of the given face (2-cell)
    std::vector<std::pair<CellIndex, CocellSense>> getEdgesOfFace(const CellIndex& face, const Body& body) { 
        std::vector<std::pair<CellIndex, CocellSense>> result;
        auto boundaries = getKBoundary(1, face, body); 
        for (const auto& edge : boundaries) {
            const CellIndex edgeIndex = edge.first;
            const CocellSense coedgeSense = edge.second;
            if (coedgeSense == 0) { 
                // discard internal edges, as a convenience to the caller
                continue;
            }
            result.push_back(edge);
        }
        return result;
    }

    // Returns all vertices (0-cells) of the given edge (1-cell). 
    std::vector<std::pair<CellIndex, CocellSense>> getVerticesOfEdge(const CellIndex& edge, const Body& body) {
        std::vector<std::pair<CellIndex, CocellSense>> result;
        auto boundaries = getKBoundary(0, edge, body);
        for (const auto& vertex : boundaries) {
            const CellIndex vertexIndex = vertex.first;
            const CocellSense covertexSense = vertex.second;
            if (covertexSense == 0) { 
                // discard internal vertices, as a convenience to the caller
                continue;
            }
            result.push_back(vertex);
        }
        if (result.size() == 2 && result[1].second == -1) {
            // flip the order so that we return the vertices in <start, end> order wrt the support direction, as a convenience to the caller
            std::reverse(result.begin(), result.end());
        }
        return result;
    }

};
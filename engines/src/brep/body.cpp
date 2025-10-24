#include <iostream>

#include "utils/geom3d.h"
#include "utils/graph.h"
#include "brep/body.h"

/***
 * The Body class represents a non-regularized pointset of any dimension.
 * A Body is composed of Cells of various dimensions.
 * Each Cell represents an open, connected subset of the geometry it lies on.
 * Each Cell is bounded by lower-dimensional cells.
 * Each Cell has an "active" flag indicating whether the cell should be considered or not.
 * Boundaries-of-boundaries are also considered boundaries of the cell.
 * The collection of cells that bound a given cell are called the cell's "boundary".
 * The collection of cells that a given cell bounds are called the cell's "star".
 * Cells are connected to their boundaries, and vice versa, by Cocells. 
 * Each cocell connects a cell to one of its boundary cells, and connects a boundary cell to one of its star cells.
 * Thus the body consists of a graph of cells connected by cocells.
 * The geometry upon which a given cell sits is called the cell's "support"
 * The subset of a cell's boundary with specified dimension k is called the cell's "k-boundary".
 * The subset of a body that is the collection of cells of dimension less than or equal to k are called the body's "k-skeleton"
 * Cocells whose boundary is one dimension lower than their star are assigned a sense according to which side their star lies on.
 * Think of each cocell as having a natural direction with respect to the cell it bounds (its star) and the boundary it lies on (its boundary).
 * The natural direction of a 0D cocell (covertex) is the tangent T of its star's support.
 * The natural direction of a 2D cocell (coface) is the normal N of its boundary's support.
 * The natural direction of a 1D cocell (coedge) is the cross product of the tangent T of its boundary's support and the normal N of its star's support, i.e. T x N.
 * The sense of cocell is +1 if its natural direction points away from its star, -1 if it points towards, and 0 if the boundary cell is an internal boundary (i.e. it bounds the same cell on both sides)
 * Bibliography:
 * Rossignac and O'Connor, "SGC: A dimension-independent model for pointsets with internal structures and 
 * incomplete boundaries", in Geometric Modeling for Product Engineering, Wozny, Turner and Preiss (eds), Elsevier, 1990.
 * Implementation notes:
 * Currently, only simple geometries are supported. Complex geometries will be supported later, probably using a "rep" concept (i.e. a pointer to an external representation).
 * It would probably be a good idea to be able to create the connectivity graph from the cells and cocells whenever needed. And perhaps to update it incrementally as cells and cocells are added/removed
 * TODO:
 * add "dimension" to cell. And body?
 * add "name" to body? And cell?
 * add "attributes" field to cell, encapsulating color, material, physical properties etc.
 * add "type" to cell, e.g. point, line, circle, plane, sphere, mesh, NURBS surface etc.
 * add "rep" to cell for complex geometries
 * use the type in the ostream operator<<
 * add all the functions like kSkeleton etc (maybe do those as non-members)
 * maybe support exotic pointsets like pierced plane - can be done by having cells that are active, but with a flag indicating that the pointset they define should be excluded from the body's pointset
 * Thinking about cache usage:
 * My 2017 iMac sysctl -a | grep "l.*cachesize" gives:
 *   hw.l1icachesize: 32768
 *   hw.l1dcachesize: 32768
 *   hw.l2cachesize: 262144
 *   hw.l3cachesize: 8388608
 * At time of writing, a Cell is 96 bytes. A cocell is 24 bytes.
 * L1d cache is 32kB, which is 32768/96 = 340 Cell objects or 1365 Cocell objects.
 * L2 cache is 256kB, which is 262144/96 = 2730 Cell objects or 10922 Cocell objects.
 * L3 cache is 8MB, which is 8388608/96 = 87381 Cell objects or 349525 Cocell objects.
 * A body with 100 cells and 200 cocells is 100*96 + 200*24 = 14kB (+16kB for the graph), which fits in L1 cache.
 * A body with 500 cells and 1000 cocells is 70kB (+82kB for the graph), which fits in L2 cache but not L1 cache.
 * For scale, a cube has 27 cells and 46 cocells.
 */

namespace e2 {
    std::ostream& operator<<(std::ostream& os, const Cell& cell) {
        os << "Cell(active=" << cell.m_active << ", support=" << cell.m_support << ")";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Cocell& cocell) {
        os << "Cocell(starCell=" << cocell.m_starCell << ", boundaryCell=" << cocell.m_boundaryCell << ", sense=" << cocell.m_sense << ")";
        return os;
    }

    size_t Body::addCell(const Cell& cell) {
        m_cells.push_back(cell);
        m_graphNeedsUpdate = true;
        return m_cells.size() - 1;
    }

    size_t Body::addCocell(const Cocell& cocell) {
        m_cocells.push_back(cocell);
        m_graphNeedsUpdate = true;
        return m_cocells.size() - 1;
    }

    void Body::updateGraph() {
        if (m_graphNeedsUpdate) {
            // For now, just rebuild the graph from scratch. Incremental update can come later.
            // Have not used vertex properties here for now, because cells[v[i].property] would be just cells[i] anyway.
            m_graph = e2::Graph(m_cells.size()); 
            for (size_t i = 0; i < m_cocells.size(); ++i) {
                const auto& cocell = m_cocells[i];
                // Edge property is the index of the cocell.
                // Edge added star->boundary by convention, but actually doesn't matter because the graph internally connects in both directions
                m_graph.addEdge(cocell.starCell(), cocell.boundaryCell(), i);
            }
            m_graphNeedsUpdate = false;
        }
    }

    std::ostream& operator<<(std::ostream& os, const Body& body) {
        os << "Body with " << body.m_cells.size() << " cells and " << body.m_cocells.size() << " cocells." << std::endl;
        for (size_t i = 0; i < body.m_cells.size(); ++i) {
            os << "  Cell " << i << ": " << body.m_cells[i] << std::endl;
        }
        for (size_t i = 0; i < body.m_cocells.size(); ++i) {
            os << "  Cocell " << i << ": " << body.m_cocells[i] << std::endl;
        }
        return os;
    }


};


#include <iostream>

#include "utils/geom3d.h"
#include "utils/graph.h"
#include "brep/body.h"

/***
 * The Body class represents a non-regularized pointset of any dimension.
 * 
 * The key concepts are Body, Cell, Cocell and Support.
 * A Body is composed of Cells of various dimensions (3D, 2D, 1D and 0D).
 * Each Cell is bounded by lower-dimensional Cells.
 * Cells and their boundary Cells are connected to each other by Cocells.
 * A Cell represents an open, connected subset of its Support.
 * Support simply refers to the geometry that a Cell lies on.
 *
 * Each Cell has an "active" flag indicating whether the cell should be considered part of the Body or not.
 * The collection of cells that bound a given cell are called the cell's "boundary".
 * The collection of cells that a given cell bounds are called the cell's "star".
 * Boundaries-of-boundaries are considered to be part of the boundary of a cell.
 * The subset of a cell's boundary with specified dimension k is called the cell's "k-boundary".
 * The subset of a body that is the collection of cells of dimension less than or equal to k are called the body's "k-skeleton"
 * 
 * As described above, a Cocell connects a star Cell to a boundary Cell.
 * A Sense is assigned to any Cocell whose boundary Cell is one dimension lower than its star Cell. 
 * Think of each Cocell as having a natural direction:
 * The natural direction of a 0D cocell (covertex) is the tangent T of its star's support.
 * The natural direction of a 2D cocell (coface) is the normal N of its boundary's support.
 * The natural direction of a 1D cocell (coedge) is the cross product of the tangent T of its boundary's support and the normal N 
 * of its star's support, i.e. T x N.
 * The sense of cocell is +1 if its natural direction points away from its star, -1 if it points towards, and 0 if the boundary cell is 
 * an internal boundary (i.e. it bounds the same cell on both sides)
 * 
 * The body may be thought of as a graph of Cells (graph vertices) connected by Cocells (graph edges). 
 * The code contains utilities for creating and manipulating such graphs.
 *
 * For now,Cells, Cocells and Attributes, once added to a Body, cannot be removed (cells can be deactivated though).
 * Hence, indexes into the Body's Cell and Cocell vectors remain valid for the lifetime of

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
 * maybe support exotic pointsets like pierced plane - can be done by having cells that are active (i.e. to be considered) but "negative" (i.e. their points are to be actively excluded)
 *
 * Some memory considerations:
 * On my MacBook Pro (2020 M1 Pro), sysctl -a | grep
 *   hw.l1icachesize: 32768
 *   hw.l1dcachesize: 32768
 *   hw.l2cachesize: 262144
 *   hw.l3cachesize: 8388608
 * At time of writing, a Cell is 96 bytes. A cocell is 24 bytes. Hence:
 * A body with 100 cells and 200 cocells is 100*96 + 200*24 = 14kB (+16kB for the graph).
 * A body with 500 cells and 1000 cocells is 70kB (+82kB for the graph).
 * For scale, a rectangular cuboid has 27 cells and 46 cocells.
 * Some people suggest 16k chunks as a good size for cache efficiency. Which is about a 4-cube-equivalent body.
 * So the approach taken here, where the cells (including their support) and cocells of each body are 
 * packed into contiguous arrays, seems like a decent balance.
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

    CellIndex Body::addCell(const Cell& cell) {
        m_cells.push_back(cell);
        m_graphNeedsUpdate = true;
        return m_cells.size() - 1;
    }

    CocellIndex Body::addCocell(const Cocell& cocell) {
        m_cocells.push_back(cocell);
        m_graphNeedsUpdate = true;
        return m_cocells.size() - 1;
    }

    void Body::attachCellAttribute(CellIndex cellIndex, std::string attributeType, Attribute* attribute){
        // they are stored by type first, then by cell index.
        // It will probably be more common to process one attribute type for many cells than vice versa.
        m_cellAttributes[attributeType][cellIndex] = attribute;
        
    }
    bool Body::findCellAttribute(CellIndex cellIndex, std::string attributeType, Attribute& outAttribute) const {
        auto it = m_cellAttributes.find(attributeType);
        if (it != m_cellAttributes.end()) {
            auto attrIt = it->second.find(cellIndex);
            if (attrIt != it->second.end()) {
                outAttribute = *attrIt->second;
                return true;
            }
        }
        return false;
    }

    void Body::updateGraph() {
        if (m_graphNeedsUpdate) {
            // For now, just rebuild the graph from scratch. Incremental update can come later.
            // Have not used vertex properties here for now, because cells[v[i].property] would be just cells[i] anyway.
            m_graph = e2::Graph(m_cells.size()); 
            for (CocellIndex i = 0; i < m_cocells.size(); ++i) {
                const auto& cocell = m_cocells[i];
                // Add the cocell as an edge in the graph, so it becomes an out-edge of its star and an in-edge of its boundary.
                m_graph.addEdge(cocell.starCell(), cocell.boundaryCell(), i);
            }
            m_graphNeedsUpdate = false;
        }
    }

    std::ostream& operator<<(std::ostream& os, const Body& body) {
        os << "Body with " << body.m_cells.size() << " cells and " << body.m_cocells.size() << " cocells." << std::endl;
        for (CellIndex i = 0; i < body.m_cells.size(); ++i) {
            os << "  Cell " << i << ": " << body.m_cells[i] << std::endl;
        }
        for (CocellIndex i = 0; i < body.m_cocells.size(); ++i) {
            os << "  Cocell " << i << ": " << body.m_cocells[i] << std::endl;
        }
        return os;
    }


};


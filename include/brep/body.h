#pragma once
#include "utils/geom3d.h"
#include "utils/graph.h"

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
 * An internal boundary (same cell on both sides) has sense 0; an external boundary has sense +1 or -1 according to which side the cell it bounds lies on.
 * The geometry upon which a given cell sits is called the cell's "support"
 * The subset of a cell's boundary with specified dimension k is called the cell's "k-boundary".
 * The subset of a body that is the collection of cells of dimension less than or equal to k are called the body's "k-skeleton"
 * Bibliography:
 * Rossignac and O'Connor, "SGC: A dimension-independent model for pointsets with internal structures and 
 * incomplete boundaries", in Geometric Modeling for Product Engineering, Wozny, Turner and Preiss (eds), Elsevier, 1990.
 * Implementation notes:
 * Currently, only simple geometries are supported. Complex geometries will be supported later, probably using a "rep" concept (i.e. a pointer to an external representation).
 * It would probably be a good idea to be able to create the connectivity graph from the cells and cocells whenever needed. And perhaps to update it incrementally as cells and cocells are added/removed
 * TODO:
 * add "dimension" to cell. And body?
 * add "name" to body? And cell?
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
    class Cell {
        public:
            Cell() {}
            Cell(const Geom3d& support, bool active = true) : m_support(support), m_active(active) {}
            bool isActive() const { return m_active; }
            const Geom3d& support() const { return m_support; }
        private:
            Geom3d m_support; // the geometry upon which the cell lies
            bool m_active = true; // whether the cell is part of the pointset or not
    };

    class Cocell {
        public:
            Cocell() {}
            Cocell(size_t starCell, size_t boundaryCell, int sense ) : m_starCell(starCell), m_boundaryCell(boundaryCell), m_sense(sense) {}
            int sense() const { return m_sense; }
            size_t starCell() const { return m_starCell; }
            size_t boundaryCell() const { return m_boundaryCell; }
        private:
            size_t m_starCell = -1; // index of the cell in the body's cells vector that this cocell is part of the boundary of
            size_t m_boundaryCell = -1; // index of the cell in the body's cells vector that this cocell bounds
            int m_sense = 0; // 0 for internal boundary, +1 or -1 for external boundary according to which side the cell it bounds lies on
    };

    class Body {
        public:
            Body() : m_graphNeedsUpdate(false) {}
            Body(const std::vector<Cell>& cells, const std::vector<Cocell>& cocells = {}) : m_cells(cells), m_cocells(cocells) {
                updateGraph();
            }
            const std::vector<Cell>& cells() const { return m_cells; }
            const std::vector<Cocell>& cocells() const { return m_cocells; }
            const e2::Graph& graph() const { return m_graph; }
            bool graphNeedsUpdate() const { return m_graphNeedsUpdate; }    
            size_t addCell(const Cell& cell) {
                m_cells.push_back(cell);
                m_graphNeedsUpdate = true;
                return m_cells.size() - 1;
            }
            size_t addCocell(const Cocell& cocell) {
                m_cocells.push_back(cocell);
                m_graphNeedsUpdate = true;
                return m_cocells.size() - 1;
            }
            void updateGraph() {
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
        private:
            std::vector<Cell> m_cells; // all the cells in the body
            std::vector<Cocell> m_cocells; // all the cocells in the body
            e2::Graph m_graph; // graph representing the connectivity of cells via cocells
            bool m_graphNeedsUpdate = true; // whether the graph needs to be rebuilt from the cells and cocells
    };
};


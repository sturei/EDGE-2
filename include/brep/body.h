#pragma once
#include "utils/geom3d.h"
#include "utils/graph.h"

namespace e2 {
    class Cell {
        public:
            Cell() {}
            Cell(const Geom3d& support, bool active = true) : m_support(support), m_active(active) {}
            bool isActive() const { return m_active; }
            const Geom3d& support() const { return m_support; }
            friend std::ostream& operator<<(std::ostream& os, const Cell& cell);
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
            friend std::ostream& operator<<(std::ostream& os, const Cocell& cocell);
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
            size_t addCell(const Cell& cell);
            size_t addCocell(const Cocell& cocell);
            void updateGraph();
            friend std::ostream& operator<<(std::ostream& os, const Body& body);
        private:
            std::vector<Cell> m_cells; // all the cells in the body
            std::vector<Cocell> m_cocells; // all the cocells in the body
            e2::Graph m_graph; // graph representing the connectivity of cells via cocells
            bool m_graphNeedsUpdate = true; // whether the graph needs to be rebuilt from the cells and cocells
    };
};


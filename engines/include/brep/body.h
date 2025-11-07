#pragma once
#include "utils/geom3d.h"
#include "utils/graph.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>


namespace e2 {

    typedef size_t CellIndex;       // Index into the Body's Cell vector
    typedef size_t CocellIndex;     // Index into the Body's Cocell vector
    typedef size_t AttributeIndex;  // Index into the Body's Attribute map
    typedef int CocellSense;        // +1 for positive, -1 for negative, 0 for neutral

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
            Cocell(CellIndex starCell, CellIndex boundaryCell, CocellSense sense ) : m_starCell(starCell), m_boundaryCell(boundaryCell), m_sense(sense) {}
            CocellSense sense() const { return m_sense; }
            CellIndex starCell() const { return m_starCell; }
            CellIndex boundaryCell() const { return m_boundaryCell; }
            friend std::ostream& operator<<(std::ostream& os, const Cocell& cocell);
        private:
            CellIndex m_starCell = -1;
            CellIndex m_boundaryCell = -1;
            CocellSense m_sense = 0;
    };

    class Attribute {
        public:
            virtual ~Attribute() {};
            virtual void print(std::ostream& os) const = 0;
            friend std::ostream& operator<<(std::ostream& os, const Attribute& a);
        };

    class Body {
        public:
            Body() : m_graphNeedsUpdate(false) {
                // destroy the attributes.
                for (auto& pair : m_cellAttributes) {
                    for (auto& attrPair : pair.second) {
                        delete attrPair.second;
                    }
                }
            }
            Body(const std::vector<Cell>& cells, const std::vector<Cocell>& cocells = {}) : m_cells(cells), m_cocells(cocells) {
                updateGraph();
            }

            const std::vector<Cell>& cells() const { return m_cells; }
            const std::vector<Cocell>& cocells() const { return m_cocells; }
            CellIndex addCell(const Cell& cell);
            const Cell& cell(CellIndex index) const { return m_cells[index]; }   
            const size_t numCells() const { return m_cells.size(); }
            CocellIndex addCocell(const Cocell& cocell);
            const Cocell& cocell(CocellIndex index) const { return m_cocells[index]; }   
            const size_t numCocells() const { return m_cocells.size(); }

            const e2::Graph& graph() const { return m_graph; }
            bool graphNeedsUpdate() const { return m_graphNeedsUpdate; }    
            void updateGraph();

            void attachCellAttribute(CellIndex cellIndex, std::string attributeType, Attribute* attribute);   // body takes ownership of attribute pointer
            bool findCellAttribute(CellIndex cellIndex, std::string attributeType, Attribute& outAttribute) const;

            friend std::ostream& operator<<(std::ostream& os, const Body& body);
        private:
            std::vector<Cell> m_cells; // all the cells in the body
            std::vector<Cocell> m_cocells; // all the cocells in the body
            std::map<std::string, std::map<CellIndex, Attribute*>> m_cellAttributes; // attributes attached to cells
            e2::Graph m_graph; // graph representing the connectivity of cells via cocells
            bool m_graphNeedsUpdate = true; // whether the graph needs to be rebuilt from the cells and cocells
    };
};


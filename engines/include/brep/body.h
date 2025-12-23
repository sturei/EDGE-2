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
    typedef int CocellSense;        // +1 for positive, -1 for negative, 0 for neutral

    class Cell {
        public:
            Cell() = default;
            Cell(const Cell&) = default;
            Cell& operator=(const Cell&) = default;
            ~Cell() = default;  
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
            Cocell() = default;
            Cocell(const Cocell&) = default;
            Cocell& operator=(const Cocell&) = default;
            ~Cocell() = default;
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
        public:;
            Attribute() = default;
            Attribute(const Attribute&) = delete;
            Attribute& operator=(const Attribute&) = delete;
            virtual ~Attribute() = default;
            virtual Attribute* clone() const = 0;   
            virtual void print(std::ostream& os) const = 0;
            friend std::ostream& operator<<(std::ostream& os, const Attribute& a);
    };

    class Body {
        public:
            Body() {
                m_graphNeedsUpdate = false;     // graph is empty but up to date
            }
            Body(const Body& other) {
                // deep copy the cells and cocells
                m_cells = other.m_cells;
                m_cocells = other.m_cocells;

                // deep copy the attributes
                for (auto& pair : other.m_cellAttributes) {
                    for (auto& attrPair : pair.second) {
                        m_cellAttributes[pair.first][attrPair.first] = attrPair.second->clone();
                    }
                
                }

                // rebuild the graph. TODO: could clone the graph if the graph supported clone()
                updateGraph();
            }
            Body& operator=(const Body& other) {
                if (this != &other) {
                    // destroy the attributes
                    for (auto& pair : m_cellAttributes) {
                        for (auto& attrPair : pair.second) {
                            delete attrPair.second;
                        }
                    }
                    // copy the contents
                    *this = other;
                }
                return *this;
            }
            ~Body() {
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
            bool findCellAttribute(CellIndex cellIndex, std::string attributeType, const Attribute*& outAttribute) const;

            friend std::ostream& operator<<(std::ostream& os, const Body& body);
        private:
            std::vector<Cell> m_cells; // all the cells in the body
            std::vector<Cocell> m_cocells; // all the cocells in the body
            std::map<std::string, std::map<CellIndex, Attribute*>> m_cellAttributes; // attributes attached to cells
            e2::Graph m_graph; // graph representing the connectivity of cells via cocells
            bool m_graphNeedsUpdate = true; // whether the graph needs to be rebuilt from the cells and cocells
    };
};


#pragma once
#include "utils/geom3d.h"
#include "utils/graph.h"

/***
 * The Body class represents a non-regularized pointset of any dimension.
 * A Body is composed of Cells of various dimensions.
 * Each Cell represents an open, connected subset of the geometry it lies on.
 * Each Cell is bounded by lower-dimensional cells.
 * Each Cell has an "active" flag indicating whether the cell should be considered part of the pointset or not.
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
 */

#if 0
namespace e2 {
    class Body {
        public:
            Body() {}
        private:
            std::vector<Cell> m_cells; // all the cells in the body
            std::vector<Cocell> m_cocells; // all the cocells in the body
            e2:::Graph m_connections; // graph representing the connectivity of cells via cocells
    };
};
#endif
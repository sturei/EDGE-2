#include "brep/brepModel.h"
#include "document/model.h"

/**
 * A BRepModel is a collection of Bodies. The BRepModel owns its own copies of the bodies it contains.
 * The expectation is that a BRepModel will be created initially with 1 or a few empty bodies, and then those bodies will be populated by subsequent state changes.
 * There will probably be a copy constructor and operator= for undo/redo support.
 * The accessor that returns a reference to the vector of bodies (an internal structure) will probably be removed.
 * Probably will change from vector<Body to vector<Body*>. Probably the best model is to have each body as packed arrays of cells and cocells, but bodies being independent.
 */

namespace e2 {
    size_t BRepModel::addBody(const Body& body) {
        m_bodies.push_back(body);
        return m_bodies.size() - 1;
    }
    void BRepModel::removeBody(size_t index) {
        // simple erase for now. Can do something more sophisticated later (e.g. swap-and-pop) if needed.
        // Note that this invalidates all indexes after the removed body.
        if (index < m_bodies.size()) {
            m_bodies.erase(m_bodies.begin() + index);
        }
    }
    void BRepModel::print(std::ostream& os) const {
        os << "BRepModel with " << m_bodies.size() << " bodies." << std::endl;
        for (size_t i = 0; i < m_bodies.size(); ++i) {
            os << "  Body " << i << ": " << m_bodies[i] << std::endl;
        }
    }
};


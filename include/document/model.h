#pragma once
#include "brep/body.h"

/**
 * The Model class represents some specific application domain.
 * Different types of models may be supported, e.g. boundary representation (BRep), constructive solid geometry (CSG), voxel models etc.
 * Implementation notes:
 * Currently, only BRep models are supported.
 * The expectation is that a BRepModel will be created initially with 1 or a few empty bodies, and then those bodies will be populated by subsequent state changes.
 * There will probably be a copy constructor and operator= for undo/redo support.
 * The accessor that returns a reference to an internal structure will probably be removed.
 */

namespace e2 {

    class Model {
    public:
        virtual ~Model() {};
        // Add pure virtual functions as needed
    };

    class BRepModel : public Model {
    public:
        BRepModel() : Model() {}
        const std::vector<Body>& bodies() const { return m_bodies; }
        size_t numBodies() const { return m_bodies.size(); }
        Body& body(size_t index) { return m_bodies[index]; }
        const Body& body(size_t index) const { return m_bodies[index]; }
        size_t addBody(const Body& body) {
            m_bodies.push_back(body);
            return m_bodies.size() - 1;
        }
        void removeBody(size_t index) {
            // simple erase for now. Can do something more sophisticated later (e.g. swap-and-pop) if needed.
            // Note that this invalidates all indexes after the removed body.
            if (index < m_bodies.size()) {
                m_bodies.erase(m_bodies.begin() + index);
            }
        }
    private:
        std::vector<Body> m_bodies; 
    };

};


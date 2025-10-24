#pragma once
#include "brep/body.h"
#include "document/model.h"

namespace e2 {
    class BRepModel : public Model {
    public:
        BRepModel() : Model() {}
        const std::vector<Body>& bodies() const { return m_bodies; }
        size_t numBodies() const { return m_bodies.size(); }
        Body& body(size_t index) { return m_bodies[index]; }
        const Body& body(size_t index) const { return m_bodies[index]; }
        size_t addBody(const Body& body);
        void removeBody(size_t index);
        void print(std::ostream& os) const override;
    private:
        std::vector<Body> m_bodies; 
    };

};


#pragma once
#include "brep/body.h"
#include "document/model.h"
#include "utils/tfm3d.h"

// Note: BRepModel owns its bodies. It takes ownership of any bodies added to it.
namespace e2 {
    class BRepModel : public Model {
    public:
        BRepModel() : Model() {}
        ~BRepModel();
        const std::vector<Body*> bodies() const { return m_bodies; }
        size_t numBodies() const { return m_bodies.size(); }
        Body* body(size_t index) { return m_bodies[index]; }
        const Body* body(size_t index) const { return m_bodies[index]; }
        size_t addBody(Body* body);
        size_t addBody(Body* body, const Tfm3d& transform);
        const Tfm3d& transform(size_t index) const { return m_transforms[index]; }
        void removeBody(size_t index);
        void print(std::ostream& os) const override;
    private:
        std::vector<Body*> m_bodies; 
        std::vector<Tfm3d> m_transforms;
    };

};


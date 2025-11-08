#pragma once
#include "brep/brepModel.h"

namespace e2 {
    class ShapeModel : public Model {
    public:
        ShapeModel(BRepModel* brepModel) : Model(), m_brepModel(brepModel) {}
        ~ShapeModel() {
            delete m_brepModel;
        }
        BRepModel* brepModel() { return m_brepModel; }
        const BRepModel* brepModel() const { return m_brepModel; }
        void print(std::ostream& os) const override;
    private:
        BRepModel* m_brepModel; 
    };

};


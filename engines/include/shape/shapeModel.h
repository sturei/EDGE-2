#pragma once
#include "brep/brepModel.h"
#include "frep/frepModel.h"

namespace e2 {
    class ShapeModel : public Model {
    public:
        ShapeModel(BRepModel* brepModel, FRepModel* frepModel) : Model(), m_brepModel(brepModel), m_frepModel(frepModel) {}
        ~ShapeModel() {
            delete m_brepModel;
            delete m_frepModel;
        }
        BRepModel* brepModel() { return m_brepModel; }
        FRepModel* frepModel() { return m_frepModel; }  
        const BRepModel* brepModel() const { return m_brepModel; }
        const FRepModel* frepModel() const { return m_frepModel; }  
        void print(std::ostream& os) const override;
    private:
        BRepModel* m_brepModel; 
        FRepModel* m_frepModel;
    };

};


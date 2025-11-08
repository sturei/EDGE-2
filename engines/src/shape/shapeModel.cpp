#include "shape/shapeModel.h"
#include "document/model.h"

/**
 * A ShapeModel contains different ways of repreenting shape. (but just Brep atm)
 */

namespace e2 {

    void ShapeModel::print(std::ostream& os) const {
        os << "ShapeModel with BRepModel." << std::endl;
        if (m_brepModel) {
            os << "  BRepModel: " << *m_brepModel  << std::endl;
        }
    }
};


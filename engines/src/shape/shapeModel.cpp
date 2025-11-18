#include "shape/shapeModel.h"
#include "document/model.h"

/**
 * A ShapeModel contains different ways of representing shape.
 */

namespace e2 {

    void ShapeModel::print(std::ostream& os) const {
        os << "ShapeModel with BRepModel and FRepModel" << std::endl;
        if (m_brepModel) {
            os << "  BRepModel: " << *m_brepModel  << std::endl;
        }
        if (m_frepModel) {
            os << "  FRepModel: " << *m_frepModel  << std::endl;
        }
    }
};


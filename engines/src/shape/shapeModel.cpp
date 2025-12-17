#include "shape/shapeModel.h"
#include "document/model.h"

/**
 * A ShapeModel contains different ways of representing shape.
 */

namespace e2 {

    void ShapeModel::print(std::ostream& os) const {
        os << "ShapeModel:" << std::endl;
        if (m_sketches) {
            os << "  Sketches (BRepModel): " << *m_sketches  << std::endl;
        }
        if (m_profiles) {
            os << "  Profiles (BRepModel): " << *m_profiles  << std::endl;
        }
        if (m_features) {
            os << "  Features (FeatureModel): " << *m_features  << std::endl;
        }
    }   
};


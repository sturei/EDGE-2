#include <iostream>
#include "document/model.h"

/**
 * Model is a base class for multiple types of models, e.g. csg models, volumetric models, graphics models etc.
 */

namespace e2 {
    std::ostream& operator<<(std::ostream& os, const Model& m) {
        m.print(os);
        return os;
    }
};


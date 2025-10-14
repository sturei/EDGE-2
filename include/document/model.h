#pragma once
#include <iostream>

/**
 * Model is a base class for multiple types of models, e.g. csg models, volumetric models, graphics models etc.
 */

namespace e2 {

    class Model {
    public:
        virtual ~Model() {};
        virtual void print(std::ostream& os) const = 0;
        friend std::ostream& operator<<(std::ostream& os, const Model& m) {
            m.print(os);
            return os;
        }
    };

};


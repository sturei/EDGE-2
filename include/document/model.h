#pragma once
#include <iostream>

namespace e2 {

    class Model {
    public:
        virtual ~Model() {};
        virtual void print(std::ostream& os) const = 0;
        friend std::ostream& operator<<(std::ostream& os, const Model& m);
    };

};


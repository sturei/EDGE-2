#pragma once
#include <array>
#include <cmath>
#include "utils/res.h"

namespace e2 {

    struct CosSinSolution {
        double cost;
        double sint;
    };

    int solveQuadraticCosSin(double a, double b, double c, std::array<CosSinSolution,2>& solutionsOut);

}
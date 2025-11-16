#include <array>
#include <cmath>
#include "utils/res.h"
#include "utils/solve.h"

namespace e2 {

    /** Solves a quadratic of the form a*cos(t) + b*sin(t) + c = 0. */
    int solveQuadraticCosSin(double a, double b, double c, std::array<CosSinSolution, 2>& solutionsOut) {
        double rsq = a*a + b*b;
        double d = rsq - c*c;
        d = d < 0 ? -sqrt(-d) : sqrt(d);

        bool az = angleZero(a);
        bool bz = angleZero(b);
        
        if (az && bz) {
            return 0;  // null or inconsistent
        }
        else if (d < -RESNOR) {
            return 0;  // no real roots
        }
        else if (d < RESNOR){
            solutionsOut[0] = {-a*c/rsq, -b*c/rsq};
            return 1;  // equal roots

        }
        else if (az) {
            solutionsOut[0] = {-d/b, -c/b};
            solutionsOut[1] = {d/b, -c/b};
            return 2;  // two real roots: b*sin(t) + c = 0
        }
        else if (bz) {
            solutionsOut[0] = {-c/a, d/a};
            solutionsOut[1] = {-c/a, -d/a};
            return 2;  // two real roots: a*cos(t) + c = 0
        }
        else {
            solutionsOut[0] = {-(b*d + a*c)/rsq, (a*d - b*c)/rsq};
            solutionsOut[1] = {(b*d - a*c)/rsq, -(a*d + b*c)/rsq};
            return 2;  // two real roots: a*cos(t) * b*sin(t) + c = 0
        }
    }

}
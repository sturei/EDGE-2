#pragma once

/**
 * RESNOR is used for comparing dimensionless quantities (typically angles). It is set several orders of magnitude larger
 * than a typical floating-point epsilon for 64-bit doubles.
 * RESABS is used for comparing length-like quantities. For example, we consider 2 points to be equal if the 
 * distance between them is less than RESABS.
 * Provided we do all our modelling within a box of side half-length SIZE, where SIZE=RESABS/RESNOR, our 2 small numbers provide 
 * sufficient protection against accumulated roundoff error.
 * Think of RESNOR as the angle subtended at the centre of a circle radius SIZE by 2 points on its circumference, 
 * separated by RESABS. 
 * The specific numbers chosen here, assuming length units of metres, imply that points less than one micron apart are considered
 * equal, and we must do all our modelling within a box of side half-length 10km.
 * It is a simple model - it does not address problems like tolerance stack-up, non-transitivity of the equality relation or
 * approximation of geometry by piecewise linear or other lower-degree approximations.
 */

namespace e2 {
    const double RESNOR = 1.0e-10;
    const double RESABS = 1.0e-6;
    const double SIZE = RESABS/RESNOR;
    const double RESABS_SQ = RESABS*RESABS;

    inline static bool angleZero(double angle) {
        return -RESNOR < angle && angle < RESNOR;
    };

    inline static bool lengthZero(double length) {
        return -RESABS < length && length < RESABS;
    };
}



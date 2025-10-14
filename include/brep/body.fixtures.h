#pragma once
#include "brep/body.h"

/**
 * BodyFixtures provides a set of fixtures for testing the Body class.
 */

namespace e2 {
    namespace BodyFixtures {
        Body* createEmptyBody() {
            // A body with no cells
            Body* emptyBody = new Body;
            return emptyBody;
        }
        Body* createAcornBody() {
            // A body containing a single point cell at (1,2,3).
            Body* acornBody = new Body({
                Cell(Point3d(Vec3d(1,2,3)))
            });
            return acornBody;
        }
        Body* createCanonicalAcornBody() {
            // A body containing a single point cell at the origin
            Body* acornBody = new Body({
                Cell(Point3d(Vec3d(0,0,0)))
            });
            return acornBody;
        }
    };

};


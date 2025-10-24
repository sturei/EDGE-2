#pragma once
#include "brep/body.h"

namespace e2 {
    namespace BodyFixtures {
        Body* createEmptyBody();
        Body* createAcornBody(const Vec3d& position = Vec3d(0,0,0));
        Body* createWireRectangle(const Vec3d& lowerLeft = Vec3d(-1,-1,0), const Vec3d& upperRight = Vec3d(1,1,0));
        Body* createSheetRectangle(const Vec3d& lowerLeft = Vec3d(-1,-1,0), const Vec3d& upperRight = Vec3d(1,1,0));
    };
};


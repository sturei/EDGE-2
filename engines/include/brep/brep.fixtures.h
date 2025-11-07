#pragma once
#include "brep/body.h"

namespace e2 {
    namespace BRepFixtures {
        Body* createEmptyBody();
        Body* createAcornBody(const Vec3d& position = Vec3d(0,0,0));
        Body* createWireRectangle(const Vec3d& lowerLeft = Vec3d(-1,-1,0), const Vec3d& upperRight = Vec3d(1,1,0));
        Body* createSheetRectangle(const Vec3d& lowerLeft = Vec3d(-1,-1,0), const Vec3d& upperRight = Vec3d(1,1,0));
        Body* createWireRoundRect(const Vec3d& lowerLeft = Vec3d(-1,-1,0), const Vec3d& upperRight = Vec3d(1,1,0), double cornerRadius = 0.2);
        Body* createSheetRoundRect(const Vec3d& lowerLeft = Vec3d(-1,-1,0), const Vec3d& upperRight = Vec3d(1,1,0), double cornerRadius = 0.2);
    };
};


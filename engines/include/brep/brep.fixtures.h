#pragma once
#include "brep/body.h"

namespace e2 {
    namespace BRepFixtures {
        Body* emptyBody();
        Body* acornBody(const Vec3d& position = Vec3d(0,0,0));
        Body* wireRectangle(const Vec3d& lowerLeft = Vec3d(-1,-1,0), const Vec3d& upperRight = Vec3d(1,1,0));
        Body* sheetRectangle(const Vec3d& lowerLeft = Vec3d(-1,-1,0), const Vec3d& upperRight = Vec3d(1,1,0));
        Body* wireRoundRect(const Vec3d& lowerLeft = Vec3d(-1,-1,0), const Vec3d& upperRight = Vec3d(1,1,0), double cornerRadius = 0.2);
        Body* sheetRoundRect(const Vec3d& lowerLeft = Vec3d(-1,-1,0), const Vec3d& upperRight = Vec3d(1,1,0), double cornerRadius = 0.2);
    };
};


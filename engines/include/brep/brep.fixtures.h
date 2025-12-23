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

        Body* circle2DSheet(const std::string& pathName = "fixtures/circle2DSheet", const std::string& displayName = "Circle 2D Sheet", double radius = 1.0);
        Body* rectangle2DSheet(const std::string& pathName = "fixtures/rectangle2DSheet", const std::string& displayName = "Rectangle 2D Sheet", double width = 3.0, double height = 2.0);
        Body* roundRect2DSheet(const std::string& pathName = "fixtures/roundRect2DSheet", const std::string& displayName = "Round Rect 2D Sheet", double width = 3.0, double height = 2.0, double cornerRadius = 0.2);

    };
};


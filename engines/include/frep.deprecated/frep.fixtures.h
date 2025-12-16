#pragma once
#include "frep.deprecated/fobject.h"
#include "brep/body.h"


namespace e2 {
    namespace FRepFixtures {
        FObject* emptyObject();
        FObject* sphere(double radius);
        FObject* block(double width, double height, double depth);
        FObject* cylinder(double radius, double depth);
        FObject* infiniteRectangle(const Vec3d& lowerLeft, const Vec3d& upperRight);
        FObject* cappedRectangle(const Vec3d& lowerLeft, const Vec3d& upperRight, double depth);
        FObject* extrudedRectangle(const Vec3d& lowerLeft, const Vec3d& upperRight, double depth);
    };
};


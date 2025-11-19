#pragma once
#include "frep/fobject.h"
#include "brep/body.h"


namespace e2 {
    namespace FRepFixtures {
        FObject* rectangle(const Vec3d& lowerLeft, const Vec3d& upperRight);
        FObject* block(double width, double height, double depth);
        FObject* infiniteExtrusionFromProfile(const Body& profileBody);
        FObject* extrusionFromProfile(const Body& profileBody, double depth);
    };
};


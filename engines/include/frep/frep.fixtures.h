#pragma once
#include "frep/fobject.h"
#include "brep/body.h"


namespace e2 {
    namespace FRepFixtures {
        FObject* emptyObject();
        FObject* sphere(double radius);
        FObject* rectangle(const Vec3d& lowerLeft, const Vec3d& upperRight);
        FObject* block(double width, double height, double depth);
        FObject* infiniteExtrudedProfile(const Body& profileBody);
        FObject* extrudedProfile(const Body& profileBody, double depth);
        FObject* extrudedProfileExact(const Body& profileBody, double depth);
    };
};


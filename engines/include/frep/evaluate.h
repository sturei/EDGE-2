#pragma once

#include "frep/fobject.h"
#include "utils/vec3d.h"
#include <iostream> 

namespace e2 {

    bool evaluateSDF(const FObject& fobject, const Vec3d& position, double& output);
    bool evaluateSDF(const FObject& fobject, FNodeIndex nodeIndex,const Vec3d& position, double& output);

}
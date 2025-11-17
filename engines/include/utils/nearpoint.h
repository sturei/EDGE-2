#pragma once
#include "utils/vec3d.h"
#include "utils/ray3d.h"
#include "utils/cir3d.h"
#include "utils/geom3d.h"
#include "utils/parameterize.h"

namespace e2 {  

    Vec3d nearpoint(const Ray3d& ray, const Vec3d& p);
    Vec3d nearpoint(const Cir3d& cir, const Vec3d& p);
    Vec3d nearpoint(const Geom3d& geom, const Vec3d& p);
    bool nearpoint(const BoundedCurve& boundedCurve, const Vec3d& p, Vec3d& pOut);

}


    
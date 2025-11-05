#pragma once
#include <iostream>
#include "utils/vec3d.h"

/**
* An infinite line represented by position and direction
*/

namespace e2 {
    class Ray3d {
        public:
            Ray3d() {}
            Ray3d(const Vec3d& position, const Vec3d& direction) : m_position(position), m_direction(direction) {}
            Vec3d position() const { return m_position; }
            Vec3d direction() const { return m_direction; }
            friend std::ostream& operator<<(std::ostream& os, const Ray3d& r) {
                os << "Position: " << r.m_position << ", Direction: " << r.m_direction;
                return os;
            }
            bool operator==(const Ray3d& other) const {
                return m_position == other.m_position && m_direction == other.m_direction;
            }
        private:
            Vec3d m_position = Vec3d(0,0,0);
            Vec3d m_direction = Vec3d(0,0,1);
    };
};


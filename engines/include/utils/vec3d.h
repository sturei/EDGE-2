#pragma once
#include <iostream>
#include <cmath>

namespace e2 {
    class Vec3d {
        public:
            Vec3d() {}
            Vec3d(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}
            double x() const { return m_x; }
            double y() const { return m_y; }
            double z() const { return m_z; }
            bool operator==(const Vec3d& other) const {
                return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
            }
            Vec3d operator+(const Vec3d& other) const {
                return Vec3d(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
            }
            Vec3d addScaled(const Vec3d& v, double s) const {
                return Vec3d(m_x + s * v.m_x, m_y + s * v.m_y, m_z + s * v.m_z);
            }
            Vec3d operator-(const Vec3d& other) const {
                return Vec3d(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
            }
            Vec3d operator*(double s) const {
                return Vec3d(m_x * s, m_y * s, m_z * s);
            }
            friend Vec3d operator*(double s, const Vec3d& v) {
                return Vec3d(v.m_x * s, v.m_y * s, v.m_z * s);
            }
            Vec3d operator/(double s) const {
                return Vec3d(m_x / s, m_y / s, m_z / s);
            }
            double dot(const Vec3d& v) const {
                return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
            }
            Vec3d cross(const Vec3d& v) const {
                return Vec3d(m_y * v.m_z - m_z * v.m_y, 
                            m_z * v.m_x - m_x * v.m_z, 
                            m_x * v.m_y - m_y * v.m_x);
            }
            Vec3d operator-() const {
                return Vec3d(-m_x, -m_y, -m_z);
            }
            Vec3d normalize() const {
                return *this / magnitude();
            }
            Vec3d orthogonalize(const Vec3d& dir) const {
                return addScaled(dir, -dot(dir));
            }
            Vec3d orthonormalize(const Vec3d& dir) const {
                return orthogonalize(dir).normalize();
            }
            Vec3d ortho() const;
            double magnitude() const {
                return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
            }
            double magnitudeSquared() const {
                return m_x * m_x + m_y * m_y + m_z * m_z;
            }

            friend std::ostream& operator<<(std::ostream& os, const Vec3d& v);

        private:
            double m_x=0, m_y=0, m_z=0;
    };
};


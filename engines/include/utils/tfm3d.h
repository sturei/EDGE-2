#pragma once
#include <iostream>
#include <cmath>
#include <array>


#include "utils/vec3d.h"

/**
* A 3D transform
*/

namespace e2 {

    // Utility functions for simple matrix and vector operations
    // (AI-generated - not checked yet!!)

    template <std::size_t Dim>
    inline std::array<double, Dim * Dim> matmul(const std::array<double, Dim * Dim>& a, const std::array<double, Dim * Dim>& b)
    {
        //static_assert(Dim >= 2 && Dim <= 4, "matmul supported only for 2x2, 3x3 or 4x4 matrices");

        std::array<double, Dim * Dim> result{};
        for (std::size_t i = 0; i < Dim; ++i)
        {
            for (std::size_t j = 0; j < Dim; ++j)
            {
                double sum = 0.0;
                for (std::size_t k = 0; k < Dim; ++k)
                    sum += a[i * Dim + k] * b[k * Dim + j];
                result[i * Dim + j] = sum;
            }
        }
        return result;
    }

    template <std::size_t Dim>
    inline std::array<double, Dim * Dim> transpose(const std::array<double, Dim * Dim>& a)
    {
        //static_assert(Dim >= 2 && Dim <= 4, "Bad matrix size for transpose");

        std::array<double, Dim * Dim> result{};
        for (std::size_t i = 0; i < Dim; ++i)
            for (std::size_t j = 0; j < Dim; ++j)
                result[i * Dim + j] = a[j * Dim + i];
        return result;
    }

    template <std::size_t Dim>
    inline std::array<double, Dim> negate(const std::array<double, Dim>& v)
    {
        std::array<double, Dim> result{};
        for (std::size_t i = 0; i < Dim; ++i)
            result[i] = -v[i];
        return result;
    }

    template <std::size_t Dim>
    inline std::array<double, Dim> vecmul(const std::array<double, Dim>& v, const std::array<double, Dim * Dim>& a)
    {
        //static_assert(Dim >= 2 && Dim <= 4, "Bad matrix size for vecmul");

        std::array<double, Dim> result{};
        for (std::size_t j = 0; j < Dim; ++j)
        {
            double sum = 0.0;
            for (std::size_t k = 0; k < Dim; ++k)
                sum += v[k] * a[k * Dim + j];
            result[j] = sum;
        }
        return result;
    }

    template <std::size_t Dim>
    inline double inner(const std::array<double, Dim * Dim>& v1, const std::array<double, Dim * Dim>& v2)
    {
        //static_assert(Dim >= 1, "Dim must be at least 1");
        // This looks wrong - needs to be vector dimension not matrix. Should call it vecinner also
        const std::size_t N = Dim * Dim;
        double sum = 0.0;
        for (std::size_t k = 0; k < N; ++k)
            sum += v1[k] * v2[k];
        return sum;
    }

    template <std::size_t Dim>
    inline std::array<double, Dim> vecnorm(std::array<double, Dim> v)
    {
        double mag = std::sqrt(inner(v, v));
        if (mag == 0.0) return v;
        for (std::size_t k = 0; k < Dim; ++k)
            v[k] /= mag;
        return v;
    }

    // TODO: come back to this later...

    class Tfm3d {
        public:
            Tfm3d() {}
            Tfm3d(const Vec3d& position, const Vec3d& angles) : m_position(position), m_angles(angles) {}
            Vec3d position() const { return m_position; }
            Vec3d angles() const { return m_angles; }
            friend std::ostream& operator<<(std::ostream& os, const Tfm3d& p) {
                os << "Position: " << p.m_position << ", Angles: " << p.m_angles;
                return os;
            }
            bool operator==(const Tfm3d& other) const {
                return m_position == other.m_position && m_angles == other.m_angles;
            }
        private:
            Vec3d m_position = Vec3d(0,0,0);
            Vec3d m_angles = Vec3d(0,0,0);
    };
};

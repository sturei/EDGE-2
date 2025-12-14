#pragma once

#include <string>
#include <sstream> 
#include <utils/vec3d.h>
#include "brep/body.h"

namespace e2 {
    
    enum class FeatureEffect {
        ADD,
        SUBTRACT,
        MODIFY
    };

    inline std::ostream& operator<<(std::ostream& os, FeatureEffect featureEffect) {
        switch (featureEffect) {
            case FeatureEffect::ADD: return os << "{+}";
            case FeatureEffect::SUBTRACT: return os << "{-}";
            case FeatureEffect::MODIFY: return os << "{m}";
            default: return os << "{?}";
        }
    }

    inline std::string toString(FeatureEffect featureEffect) {
        std::stringstream ss;
        ss << featureEffect;
        return ss.str();
    }

    class Feature {
        public:
            Feature() = default;
            virtual ~Feature() = default;
            Feature(const Feature&) = delete;
            void operator=(const Feature&) = delete;
            Feature(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect);
            const std::string& pathname() const { return m_pathname; }
            const std::string& displayName() const { return m_displayName; }
            std::string displayEffect() const { return toString(m_featureEffect); } 
            virtual std::string displayType() const = 0;
            FeatureEffect featureEffect() const { return m_featureEffect; }
            friend std::ostream& operator<<(std::ostream& os, const Feature& feature);
            virtual void print(std::ostream& os) const = 0;
        private:
            std::string m_pathname;                     // the pathname serves as a unique identifier
            std::string m_displayName;                  // user-friendly name for the feature
            FeatureEffect m_featureEffect;              // whether this feature adds, subtracts, or modifies geometry
    };

    class Primitive : public Feature {
        public:
            Primitive() = default;
            virtual ~Primitive() = default;
            Primitive(const Primitive&) = delete;
            void operator=(const Primitive&) = delete;
            Primitive(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation);
            const Vec3d& position() const { return m_position; }
            const Vec3d& rotation() const { return m_rotation; } // Euler angles (in radians) representing rotation about the X, Y, and Z axes      
        private:
            Vec3d m_position;
            Vec3d m_rotation;         // Euler angles (in radians) representing rotation about the X, Y, and Z axes     
    };
    
    class Block : public Primitive {
        public:
            Block() = default;
            virtual ~Block() = default;
            Block(const Block&) = delete;
            void operator=(const Block&) = delete;
            Block(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, double width, double height, double depth);
            double width() const { return m_width; }
            double height() const { return m_height; }
            double depth() const { return m_depth; }
            std::string displayType() const override { return "Block"; }
            void print(std::ostream& os) const override;
        private:
            double m_width;
            double m_height;
            double m_depth;
    };

    class Sphere : public Primitive {
        public:
            Sphere() = default;
            virtual ~Sphere() = default;
            Sphere(const Sphere&) = delete;
            void operator=(const Sphere&) = delete;
            Sphere(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, double radius);
            double radius() const { return m_radius; }
            std::string displayType() const override { return "Sphere"; }
            void print(std::ostream& os) const override;
        private:
            double m_radius;
    };

    class Cylinder : public Primitive {
        public:
            Cylinder() = default;
            virtual ~Cylinder() = default;
            Cylinder(const Cylinder&) = delete;
            void operator=(const Cylinder&) = delete;
            Cylinder(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, double radius, double depth);
            double radius() const { return m_radius; }
            double depth() const { return m_depth; }
            std::string displayType() const override { return "Cylinder"; }
            void print(std::ostream& os) const override;
        private:
            double m_radius;
            double m_depth;
    };

    class Profile : public Feature {
        public:
            Profile() = default;
            virtual ~Profile() = default;
            Profile(const Profile&) = delete;
            void operator=(const Profile&) = delete;
            Profile(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation);
            const Vec3d& position() const { return m_position; }
            const Vec3d& rotation() const { return m_rotation; } // Euler angles (in radians) representing rotation about the X, Y, and Z axes      
        private:
            Vec3d m_position;
            Vec3d m_rotation;         // Euler angles (in radians) representing rotation about the X, Y, and Z axes 
    };

    class Primitive2D : public Profile {
        public:
            Primitive2D() = default;
            virtual ~Primitive2D() = default;
            Primitive2D(const Primitive2D&) = delete;
            void operator=(const Primitive2D&) = delete;
            Primitive2D(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation, const Vec3d& position2D, double rotation2D);
        private:
            Vec3d m_position2D;      // 2D position in the profile plane
            double m_rotation2D;     // rotation (in radians)in the profile plane
    };

    class Rectangle2D : public Primitive2D {
        public:
            Rectangle2D() = default;
            virtual ~Rectangle2D() = default;
            Rectangle2D(const Rectangle2D&) = delete;
            void operator=(const Rectangle2D&) = delete;
            Rectangle2D(
                const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
                const Vec3d& position, const Vec3d& rotation, 
                const Vec3d& position2D, double rotation2D, 
                double width, double height);

            double width() const { return m_width; }
            double height() const { return m_height; }
            std::string displayType() const override { return "Rectangle2D"; }  
            void print(std::ostream& os) const override;
        private:
            double m_width;
            double m_height;
    };

    class Circle2D : public Primitive2D {
        public:
            Circle2D() = default;
            virtual ~Circle2D() = default;
            Circle2D(const Circle2D&) = delete;
            void operator=(const Circle2D&) = delete;
            Circle2D(
                const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
                const Vec3d& position, const Vec3d& rotation, 
                const Vec3d& position2D, double rotation2D, 
                double radius);
            double radius() const { return m_radius; }
            std::string displayType() const override { return "Circle2D"; }  
            void print(std::ostream& os) const override;
        private:
            double m_radius;
    };

    class RoundRect2D : public Primitive2D {
        public:
            RoundRect2D() = default;
            virtual ~RoundRect2D() = default;
            RoundRect2D(const RoundRect2D&) = delete;
            void operator=(const RoundRect2D&) = delete;
            RoundRect2D(
                const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
                const Vec3d& position, const Vec3d& rotation, 
                const Vec3d& position2D, double rotation2D, 
                double width, double height, double cornerRadius);
            double width() const { return m_width; }
            double height() const { return m_height; }
            double cornerRadius() const { return m_cornerRadius; }
            std::string displayType() const override { return "RoundRect2D"; }
            void print(std::ostream& os) const override;
        private:
            double m_width;
            double m_height;
            double m_cornerRadius;
    };

    class Extrusion : public Feature {
        public:
            Extrusion() = default;
            virtual ~Extrusion() = default;
            Extrusion(const Extrusion&) = delete;
            void operator=(const Extrusion&) = delete;
            Extrusion(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
                const std::string& profilePathname, double depth);
            const std::string& profilePathname() const { return m_profilePathname; }
            double depth() const { return m_depth; }
            std::string displayType() const override { return "Extrusion"; }
            void print(std::ostream& os) const override;
        private:
            std::string m_profilePathname;
            double m_depth;                     // extrusion depth +/- depth/2 along the profile normal. Later this may change to reference a second profile or workplane
    };
}

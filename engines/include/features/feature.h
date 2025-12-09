#pragma once

#include <string>
#include <sstream> 
#include <utils/vec3d.h>

namespace e2 {
    
    enum class FeatureType {
        PRIMITIVE,
        EXTRUSION
    };

    inline std::ostream& operator<<(std::ostream& os, FeatureType featureType) {
        switch (featureType) {
            case FeatureType::PRIMITIVE: return os << "primitive";
            case FeatureType::EXTRUSION: return os << "extrusion";
            default: return os << "unknown";
        }
    }

    inline std::string toString(FeatureType featureType) {
        std::stringstream ss;
        ss << featureType;
        return ss.str();
    }

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

            Feature(const std::string& pathname, const std::string& displayName, FeatureType featureType, FeatureEffect featureEffect);

            const std::string& pathname() const { return m_pathname; }
            const std::string& displayName() const { return m_displayName; }
            FeatureType featureType() const { return m_featureType; }
            FeatureEffect featureEffect() const { return m_featureEffect; }

            friend std::ostream& operator<<(std::ostream& os, const Feature& feature);
            virtual void print(std::ostream& os) const = 0;
        
        private:
            std::string m_pathname;                     // the pathname serves as a unique identifier
            std::string m_displayName;                  // user-friendly name for the feature
            FeatureType m_featureType;
            FeatureEffect m_featureEffect;              // whether this feature adds, subtracts, or modifies geometry
    };

    enum class PrimitiveType {
        SPHERE,
        BLOCK,
        CYLINDER
    };

    inline std::ostream& operator<<(std::ostream& os, PrimitiveType primitiveType) {  
        switch (primitiveType) {
            case PrimitiveType::SPHERE: return os << "Sphere";
            case PrimitiveType::BLOCK: return os << "Block";
            case PrimitiveType::CYLINDER: return os << "Cylinder";
            default: return os << "Unknown";
        }
    };

    class Primitive : public Feature {
        public:
            Primitive() = default;
            virtual ~Primitive() = default;
            Primitive(const Primitive&) = delete;
            void operator=(const Primitive&) = delete;

            Primitive(const std::string& pathname, const std::string& displayName, PrimitiveType primitiveType, const Vec3d& position, const Vec3d& rotation);

            PrimitiveType primitiveType() const { return m_primitiveType; }
            const Vec3d& position() const { return m_position; }
            const Vec3d& rotation() const { return m_rotation; } // Euler angles (in radians) representing rotation about the X, Y, and Z axes      

        private:
            PrimitiveType m_primitiveType;
            Vec3d m_position;
            Vec3d m_rotation;         // Euler angles (in radians) representing rotation about the X, Y, and Z axes     
    };
    
    class Block : public Primitive {
        public:
            Block() = default;
            virtual ~Block() = default;
            Block(const Block&) = delete;
            void operator=(const Block&) = delete;

            Block(const std::string& pathname, const std::string& displayName, double width, double height, double depth);

            double width() const { return m_width; }
            double height() const { return m_height; }
            double depth() const { return m_depth; }

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

            Sphere(const std::string& pathname, const std::string& displayName, double radius);

            double radius() const { return m_radius; }

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

            Cylinder(const std::string& pathname, const std::string& displayName, double radius, double depth);

            double radius() const { return m_radius; }
            double depth() const { return m_depth; }

            void print(std::ostream& os) const override;

        private:
            double m_radius;
            double m_depth;
    };
}

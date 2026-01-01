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

    static inline std::ostream& operator<<(std::ostream& os, FeatureEffect featureEffect) {
        switch (featureEffect) {
            case FeatureEffect::ADD: return os << "{+}";
            case FeatureEffect::SUBTRACT: return os << "{-}";
            case FeatureEffect::MODIFY: return os << "{m}";
            default: return os << "{?}";
        }
    }

    static inline std::string toString(FeatureEffect featureEffect) {
        std::stringstream ss;
        ss << featureEffect;
        return ss.str();
    }

    enum class FillType {
        SPHERE,             // an example of a repeating primmitive - in practical applications it would be more likely a lattice cell
        GYROID              // an example of a continuous pattern
    };

    static inline std::ostream& operator<<(std::ostream& os, FillType fillType) {
        switch (fillType) {
            case FillType::SPHERE: return os << "SPHERE";
            case FillType::GYROID: return os << "GYROID";
            default: return os << "UNKNOWN";
        }
    }   

    class Feature {
        public:
            Feature() = default;
            virtual ~Feature() = default;
            Feature(const Feature&) = delete;
            void operator=(const Feature&) = delete;
            Feature(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation);
            const std::string& pathname() const { return m_pathname; }
            const std::string& displayName() const { return m_displayName; }
            std::string displayEffect() const { return toString(m_featureEffect); } 
            virtual std::string displayType() const = 0;
            FeatureEffect featureEffect() const { return m_featureEffect; }
            const Vec3d& position() const { return m_position; }
            const Vec3d& rotation() const { return m_rotation; }
            friend std::ostream& operator<<(std::ostream& os, const Feature& feature);
            virtual void print(std::ostream& os) const = 0;
        private:
            std::string m_pathname;                     // the pathname serves as a unique identifier for the feature
            std::string m_displayName;                  // user-friendly name for the feature
            FeatureEffect m_featureEffect;              // whether this feature adds, subtracts, or modifies geometry
            Vec3d m_position;                           // Translation vector from the canonical position of the feature. 
            Vec3d m_rotation;                           // Euler angles (in radians) representing rotation about the X, Y, and Z axes repsectively, from the canonical orientation of the feature.    

    };

    class Primitive : public Feature {
        public:
            Primitive() = default;
            virtual ~Primitive() = default;
            Primitive(const Primitive&) = delete;
            void operator=(const Primitive&) = delete;
            Primitive(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation);
        private:
    };
    
    class Block : public Primitive {
        public:
            Block() = default;
            virtual ~Block() = default;
            Block(const Block&) = delete;
            void operator=(const Block&) = delete;
            Block(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation, double width, double height, double depth);
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
            Sphere(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation, double radius);
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
            Cylinder(const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, const Vec3d& position, const Vec3d& rotation, double radius, double depth);
            double radius() const { return m_radius; }
            double depth() const { return m_depth; }
            std::string displayType() const override { return "Cylinder"; }
            void print(std::ostream& os) const override;
        private:
            double m_radius;
            double m_depth;
    };

    class Workplane : public Feature {
        public:
            Workplane() = default;
            virtual ~Workplane() = default;
            Workplane(const Workplane&) = delete;
            void operator=(const Workplane&) = delete;
            Workplane(const std::string& pathname, const std::string& displayName, const Vec3d& position, const Vec3d& rotation);
            std::string displayType() const override { return "Workplane"; }
            void print(std::ostream& os) const override;
        private:
    };  

    class Feature2D : public Feature {
        public:
            Feature2D() = default;
            virtual ~Feature2D() = default;
            Feature2D(const Feature2D&) = delete;
            void operator=(const Feature2D  &) = delete;
            Feature2D (const std::string& pathname, const std::string& displayName, const Vec3d& position, const Vec3d& rotation, const std::string& workplanePathName, const Vec3d& position2D, double rotation2D);
            const std::string& workplanePathName() const { return m_workplanePathName; }
            const Vec3d& position2D() const { return m_position2D; }
            double rotation2D() const { return m_rotation2D; }
        private:
            std::string m_workplanePathName;
            Vec3d m_position2D;      // 2D position in the profile's workplane 
            double m_rotation2D;     // rotation (in radians) in the profile's workplane

    };

    class Primitive2D : public Feature2D {
        public:
            Primitive2D() = default;
            virtual ~Primitive2D() = default;
            Primitive2D(const Primitive2D&) = delete;
            void operator=(const Primitive2D&) = delete;
            Primitive2D(const std::string& pathname, const std::string& displayName, const Vec3d& position, const Vec3d& rotation,const std::string& workplanePathName, const Vec3d& position2D, double rotation2D);
        private:
    };

    class Rectangle2D : public Primitive2D {
        public:
            Rectangle2D() = default;
            virtual ~Rectangle2D() = default;
            Rectangle2D(const Rectangle2D&) = delete;
            void operator=(const Rectangle2D&) = delete;
            Rectangle2D(
                const std::string& pathname, const std::string& displayName, const Vec3d& position, const Vec3d& rotation,
                const std::string& workplanePathName, const Vec3d& position2D, double rotation2D, 
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
                const std::string& pathname, const std::string& displayName, const Vec3d& position, const Vec3d& rotation,
                const std::string& workplanePathName, const Vec3d& position2D, double rotation2D, 
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
                const std::string& pathname, const std::string& displayName, const Vec3d& position, const Vec3d& rotation,
                const std::string& workplanePathName, const Vec3d& position2D, double rotation2D, 
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

    // Future: Profile2D feature could go here (extends Feature2D)?
    // Maybe also Sketch2D feature could go here (extends Feature2D)?

    class Extrusion : public Feature {
        public:
            Extrusion() = default;
            virtual ~Extrusion() = default;
            Extrusion(const Extrusion&) = delete;
            void operator=(const Extrusion&) = delete;
            Extrusion(
                const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect, 
                const Vec3d& position, const Vec3d& rotation,   // TOGO? Not used - would allow positioning of the extrusion feature disconnected from its profile, which seems always wrong.
                const std::string& profilePathName, double depth, bool doubleSided);
            const std::string& profilePathName() const { return m_profilePathName; }
            double depth() const { return m_depth; }
            bool doubleSided() const { return m_doubleSided; }
            std::string displayType() const override { return "Extrusion"; }
            void print(std::ostream& os) const override;
        private:
            std::string m_profilePathName;
            double m_depth;                     // extrusion depth along the profile normal.
            bool m_doubleSided;                 // in case the extrusion is double-sided, the extrusion is depth/2 in both directions along the profile normal.
    };

    class Fill : public Feature {
        public:
            Fill() = default;
            virtual ~Fill() = default;
            Fill(const Fill&) = delete;
            void operator=(const Fill&) = delete;
            Fill(
                const std::string& pathname, const std::string& displayName, FeatureEffect featureEffect,
                const Vec3d& position, const Vec3d& rotation,
                const std::string& targetPathName, FillType fillType, double cellSize);
            const std::string& targetPathName() const { return m_targetPathName; }
            double cellSize() const { return m_cellSize; }
            FillType fillType() const { return m_fillType; }
            std::string displayType() const override { return "Fill"; }
            void print(std::ostream& os) const override;
        private:
            std::string m_targetPathName;
            double m_cellSize;
            FillType m_fillType;
    };
}

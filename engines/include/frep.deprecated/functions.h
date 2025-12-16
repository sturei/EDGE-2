#pragma once

#include "frep.deprecated/fobject.h"
#include "utils/vec3d.h"
#include "utils/pla3d.h"
#include "utils/sph3d.h"
#include "brep/body.h"

#include <iostream> 

namespace e2 {

    class FSphere : public Function {
        public:
            FSphere() = default;
            FSphere(const FSphere& other) : Function(), m_sphere(other.m_sphere) {}
            Function* clone() const override { return new FSphere(*this); }

            FSphere(const Sph3d& sphere);
            bool evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
        private:
            Sph3d m_sphere;
    };

    class FBlock : public Function {
        public:
            FBlock() = default;
            FBlock(const FBlock& other) : Function(), m_width(other.m_width), m_height(other.m_height), m_depth(other.m_depth) {}
            Function* clone() const override { return new FBlock(*this); }

            FBlock(double width, double height, double depth);
            bool evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
        private:
            double m_width;
            double m_height;
            double m_depth;
    };

    class FCylinder : public Function {
        public:
            FCylinder() = default;
            FCylinder(const FCylinder& other) : Function(), m_radius(other.m_radius), m_depth(other.m_depth) {}
            Function* clone() const override { return new FCylinder(*this); }

            FCylinder(double radius, double depth);
            bool evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
        private:
            double m_radius;
            double m_depth;
    };

    class FProfileSDF : public Function {
        public:
            FProfileSDF() = default;
            FProfileSDF(const FProfileSDF& other) : Function(), m_profile(other.m_profile) {}   
            Function* clone() const override { return new FProfileSDF(*this); }

            FProfileSDF(const Body& profile);
            bool evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
        private:
            const Body m_profile;           // copy of the body defining the profile. Eventually there will something like a BodyHandle or BodyIndex or name alongside, referencing the original, and an update step to sync changes
    };

    class FExtrusionSDF : public Function {
        public:
            FExtrusionSDF() = default;
            FExtrusionSDF(const FExtrusionSDF& other) : Function(), m_depth(other.m_depth) {}
            Function* clone() const override { return new FExtrusionSDF(*this); }

            FExtrusionSDF(double depth);
            bool evaluate(const Vec3d& positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
        private:
            double m_depth;
    };

    class FHalfSpace : public Function {
        public:
            FHalfSpace() = default;
            FHalfSpace(const FHalfSpace& other) : Function(), m_plane(other.m_plane) {}
            Function* clone() const override { return new FHalfSpace(*this); }  
            FHalfSpace(const Pla3d& plane);

            bool evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
        private:
            Pla3d m_plane;
    };

    class FMin : public Function {
        public:
            FMin() = default;
            FMin(const FMin&) : Function() {};
            Function* clone() const override { return new FMin(*this); }

            bool evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
    };

    typedef class FMin FUnion;
 
    class FMax : public Function {
    public:
        FMax() = default;
        FMax(const FMax&) : Function() {};
        Function* clone() const override { return new FMax(*this); }

        bool evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
        void print(std::ostream& os) const override;
    };

    typedef class FMax FIntersection;

    class FNegation : public Function {
        public:
            FNegation() = default;
            FNegation(const FNegation&) : Function() {};
            Function* clone() const override { return new FNegation(*this); }

            bool evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
    };

    typedef class FNegation FComplement;

    class FFObject : public Function {
        public:
            FFObject() = default;
            FFObject(const FFObject& other) : Function(), m_fobject(other.m_fobject) {}
            Function* clone() const override { return new FFObject(*this); }

            FFObject(const FObject& fobject);
            bool evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
        private:
            const FObject m_fobject;               // copy of the object defining the function. Eventually there will something like an FObjectHandle or FObjectIndex or name alongside, referencing the original, and an update step to sync changes
    };

    class FConstant : public Function {
        public:
            FConstant() = default;
            FConstant(const FConstant& other) : Function(), m_value(other.m_value) {}
            Function* clone() const override { return new FConstant(*this); }

            FConstant(double value);
            bool evaluate(const Vec3d& _positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
        private:
            double m_value;
    };

    // set-theoretic operations, assuming set is defined as all P where f(P) <= 0

}
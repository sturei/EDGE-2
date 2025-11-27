#pragma once

#include "frep/fobject.h"
#include "utils/vec3d.h"
#include "utils/pla3d.h"
#include "brep/body.h"

#include <iostream> 

namespace e2 {

    class FMax : public Function {
    public:
        bool evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
        void print(std::ostream& os) const override;
    };


    class FMin : public Function {
        public:
            bool evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
    };

    class FNegation : public Function {
        public:
            bool evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
    };

    class FConstant : public Function {
        public:
            FConstant(double value) : m_value(value) {}
            bool evaluate(const Vec3d& _positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
        private:
            double m_value;
    };

    class FHalfSpace : public Function {
        public:
            FHalfSpace(const Pla3d& plane) : m_plane(plane) {}
            bool evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
        private:
            Pla3d m_plane;
    };

    class FFObject : public Function {
        public:
            FFObject(const FObject& fobject) : m_fobject(fobject) {}
            bool evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
        private:
            const FObject m_fobject;               // copy of the object defining the function. Eventually there will something like an FObjectHandle or FObjectIndex or name alongside, referencing the original, and an update step to sync changes
    };

    class FProfileSDF : public Function {
    public:
        FProfileSDF(const Body& profile) : m_profile(profile) {}
        bool evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
        void print(std::ostream& os) const override;
    private:
        const Body m_profile;           // copy of the body defining the profile. Eventually there will something like a BodyHandle or BodyIndex or name alongside, referencing the original, and an update step to sync changes
    };

    class FExtrusionSDF : public Function {
    public:
        FExtrusionSDF(double depth) : m_Depth(depth) {}
        bool evaluate(const Vec3d& positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
        void print(std::ostream& os) const override;
    private:
        double m_Depth;           // depth of the extrusion. TODO: this should take 2 parameters (workplane bodies) and create and extrusion between them (measured length plus offset in the transform)
    };


    // set-theoretic operations, assuming set is defined as all P where f(P) <= 0
    typedef class FMin FUnion;
    typedef class FMax FIntersection;
    typedef class FNegation FComplement;

}
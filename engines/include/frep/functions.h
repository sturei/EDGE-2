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
            const FObject& m_fobject;               // reference to another object in the same model
    };

    class FProfileSDF : public Function {
    public:
        FProfileSDF(const Body& body) : m_Body(body) {}
        bool evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
        void print(std::ostream& os) const override;
    private:
        const Body& m_Body;        // reference to the body defining the profile. TODO: think about the best way to have such an external reference.
    };

    class FExtrusionSDF : public Function {
    public:
        FExtrusionSDF(double depth) : m_Depth(depth) {}
        bool evaluate(const Vec3d& positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
        void print(std::ostream& os) const override;
    private:
        double m_Depth;           // depth of the extrusion
    };


    // set-theoretic operations, assuming set is defined as all P where f(P) <= 0
    typedef class FMin FUnion;
    typedef class FMax FIntersection;
    typedef class FNegation FComplement;

}
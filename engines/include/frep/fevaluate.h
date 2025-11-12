#pragma once

#include "frep/fobject.h"
#include "utils/vec3d.h"
#include <iostream> 

namespace e2 {

    class FMax : public FEvaluator {
    public:
        bool evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
        void print(std::ostream& os) const override;
    };


    class FMin : public FEvaluator {
        public:
            bool evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
    };

    class FNegation : public FEvaluator {
        public:
            bool evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
    };

    class FConstant : public FEvaluator {
        public:
            FConstant(double value) : m_value(value) {}
            bool evaluate(const Vec3d& _positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
            void print(std::ostream& os) const override;
        private:
            double m_value;
    };

    // set-theoretic operations, assuming set is defined as all P where f(P) <= 0
    typedef class FMin FUnion;
    typedef class FMax FIntersection;
    typedef class FNegation FComplement;

    // evaluations on FObjects
    bool evaluate(const FObject& fobject, const Vec3d& position, double& output);
    bool evaluate(const FObject& fobject, FNodeIndex nodeIndex,const Vec3d& position, double& output);

}
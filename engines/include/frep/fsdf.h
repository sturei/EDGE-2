#pragma once

#include "frep/functions.h"
#include "brep/body.h"

namespace e2 {

    class FProfileSDF : public FEvaluator {
    public:
        FProfileSDF(const Body& body) : m_Body(body) {}
        bool evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
        void print(std::ostream& os) const override;
    private:
        const Body& m_Body;           // reference to the body defining the profile. In same store, hence same lifetime, as the FObject using this evaluator.
                                        // An alternative would be store a (persistent) name of the the body, if such a thing existed
    };

    class FExtrusionSDF : public FEvaluator {
    public:
        FExtrusionSDF(double depth) : m_Depth(depth) {}
        bool evaluate(const Vec3d& positionIn, const std::vector<double>& argsIn, double& valueOut) const override;
        void print(std::ostream& os) const override;
    private:
        double m_Depth;           // depth of the extrusion
    };


}



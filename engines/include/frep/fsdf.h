#pragma once

#include "frep/fevaluate.h"
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

}



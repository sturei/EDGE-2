#pragma once

#include "frep/fevaluate.h"
#include "brep/body.h"

namespace e2 {

    class SDFToProfile : public FEvaluator {
    public:
        SDFToProfile(const Body& body) : m_Body(body) {}
        bool evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const override;
        void print(std::ostream& os) const override;
    private:
        const Body& m_Body;    
    };

}



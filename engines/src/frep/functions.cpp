    
#include "frep/fobject.h"
#include "frep/fnavigate.h"
#include "utils/vec3d.h"
#include "frep/functions.h"    
#include <iostream> 

namespace e2 {

    // Evaluates the max of its input arguments
    bool FMax::evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const {
        if (argsIn.empty()) {
            return false;
        }
        valueOut = *std::max_element(argsIn.begin(), argsIn.end());
        //std::cerr << "FMax::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FMax::print(std::ostream& os) const {
        os << "FMax";
    }

    // Evaluates the min of its input arguments
    bool FMin::evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const {
        if (argsIn.empty()) {
            return false;
        }
        valueOut = *std::min_element(argsIn.begin(), argsIn.end());
        //std::cerr << "FMin::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FMin::print(std::ostream& os) const {
        os << "FMin";
    }

    // Evaluates the negation of its input argument
    bool FNegation::evaluate(const Vec3d& _positionIn, const std::vector<double>& argsIn, double& valueOut) const {
        if (argsIn.size() != 1) {
            return false;
        }
        valueOut = -argsIn[0];
        //std::cerr << "FNegation::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FNegation::print(std::ostream& os) const {
        os << "FNegation";
    }

    // Evaluates to a constant value
    bool FConstant::evaluate(const Vec3d& _positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        valueOut = m_value;
        //std::cerr << "FConstant::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }
    
    void FConstant::print(std::ostream& os) const {
        os << "FConstant(" << m_value << ")";
    }

    // Evaluates to the distance to a plane
    bool FHalfSpace::evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        valueOut = (positionIn - m_plane.position()).dot(m_plane.normal());
        //std::cerr << "FHalfSpace::evaluate: valueOut = " << valueOut << std::endl;
        return true;
    }

    void FHalfSpace::print(std::ostream& os) const {
        os << "FHalfSpace(Plane: " << m_plane << ")";
    }

    // Evaluates to the value of the wrapped object
    bool FFObject::evaluate(const Vec3d& positionIn, const std::vector<double>& _argsIn, double& valueOut) const {
        bool result = e2::evaluate(m_fobject, positionIn, valueOut);
        //std::cerr << "FFObject::evaluate: valueOut = " << valueOut << std::endl;
        return result;
    }

    void FFObject::print(std::ostream& os) const {
        os << "FFObject wrapping FObject";
    }

    // Evaluates the fobject at the given position, starting from the root node
    bool evaluate(const FObject& fobject, const Vec3d& position, double& output) {
        FNodeIndex rootIndex = fobject.rootIndex();
        bool result = evaluate(fobject, rootIndex, position, output);
        //std::cerr << "evaluate(FObject) at " << position << ": output = " << output << std::endl;
        return result;
    }

    // Evaluates the fobject at the given position, starting from the specified node
    bool evaluate(const FObject& fobject, FNodeIndex nodeIndex,const Vec3d& position, double& output) {
        
        // gather input values
        const auto inputNodes = getInputFNodes(fobject, nodeIndex);
        std::vector<double> inputValues;
        for (const auto& inputNodeIndex : inputNodes) {
            double inputValue = 0.0;
            if (evaluate(fobject, inputNodeIndex, position, inputValue)) {
                inputValues.push_back(inputValue);
            }
        }

        // evaluate this node
        const FNode& node = fobject.fnode(nodeIndex);
        bool result = false;
        Function* function = fobject.function(nodeIndex);
        if (function) {
            result = function->evaluate(position, inputValues, output);
        } else {    
            // Oops - no function
            std::cerr << "No function found for " << node << std::endl;
        }

        return result;
    }

}
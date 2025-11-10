    
#include "frep/fobject.h"
#include "frep/navigate.h"
#include "utils/vec3d.h"
#include "frep/evaluate.h"    
#include <iostream> 

namespace e2 {

    bool evaluateSDF(const FObject& fobject, const Vec3d& position, double& output) {
        FNodeIndex rootIndex = fobject.rootIndex();
        return evaluateSDF(fobject, rootIndex, position, output);
    }

    bool evaluateSDF(const FObject& fobject, FNodeIndex nodeIndex,const Vec3d& position, double& output) {
        // Implementation for evaluating the SDF at a specific node

        bool result = false;
        FEvaluator* evaluator = nullptr;
        const FNode& node = fobject.fnode(nodeIndex);
        const auto inputNodes = getInputFNodes(fobject, nodeIndex);
        double maxValue = -std::numeric_limits<double>::infinity();
        double minValue = std::numeric_limits<double>::infinity();

        switch (node.type()) {
            case FNodeType::MAX:
                // Return the MAX of all the valid inputs
                for (const auto& inputNodeIndex : inputNodes) {
                    double inputValue = 0.0;
                    if (evaluateSDF(fobject, inputNodeIndex, position, inputValue)) {
                        if (inputValue > maxValue) {
                            maxValue = inputValue;
                            result = true;             // we found at least one valid input
                        }
                    }
                }
                if (result) {
                    output = maxValue;
                }
                break;
            case FNodeType::MIN:
                // Return the MIN of all the valid inputs
                for (const auto& inputNodeIndex : inputNodes) {
                    double inputValue = 0.0;
                    if (evaluateSDF(fobject, inputNodeIndex, position, inputValue)) {
                        if (inputValue < minValue) {
                            minValue = inputValue;
                            result = true;             // we found at least one valid input
                        }
                    }
                }
                if (result) {
                    output = minValue;
                }
                break;
            case FNodeType::NEGATION:
                // Return the negation of the first valid input
                for (const auto& inputNodeIndex : inputNodes) {
                    double inputValue = 0.0;
                    if (evaluateSDF(fobject, inputNodeIndex, position, inputValue)) {
                        output = -inputValue;
                        result = true;
                        break; // only one input expected
                    }
                }
                break;
            case FNodeType::EVALUATION:
                if (fobject.findEvaluator(nodeIndex, evaluator)) {
                    result = evaluator->evaluateSDF(fobject, nodeIndex, position, output);
                } else {    
                    // Oops - EVALUATION type without an evaluator attached
                    std::cerr << "No evaluator found for " << node << std::endl;
                }
                break;
            default:
                // Unknown node type
                std::cerr << "Unknown FNodeType for " << node << std::endl;
                break;
        }
        return result;
    }

}
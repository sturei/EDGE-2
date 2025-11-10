#include <iostream>

#include "utils/graph.h"
#include "frep/fobject.h"

/***
 * The FObject class represents a closed subset of n-dimensional space.
 * It defines the above as the set of points P where f(P) <= 0, f being a continuous 
 * real function of n variables.
 * 
 * The key concepts of the implementation are FNode, FArg and FEvaluator.
 * 
 * A function f(p) is either atomic or composed of other functions. FNodes represent these functions 
 * and FArgs represent the arguments to these functions (which are in turn the output of other functions).
 * 
 * In general, an FNode can have multiple input arguments (represented by multiple FArgs) and its value (output) 
 * may be used as the input to multiple other FNodes (also represented by multiple FArgs).
 * Thus we obtain a directed acyclic graph of FNodes connected by FArgs. This graph is represented explicitly 
 * using the Graph class attached to the FObject. The graph can rebuilt from the FNodes and FArgs 
 * whenever needed.
 * 
 * FNodes come with a few built-in types, such as MAX (which returns the maximum of its n arguments, 
 * implementing set intersection). Arbitrary functions may be implemented by subclassing the FEvaluator class and
 * attaching an instance of the subclassed evaluator to an FNode (and setting its type to EVALUATION).
 *
 * FNodes and FArgs, once added to a FObject, cannot be removed (FNodes can be deactivated though).
 * Hence, indexes into the FObject's FNode and FArg vectors remain valid for the lifetime of the FObject.

 * Bibliography:
 * Pasko, Adzhiev, Sourin and Savchenko, "Function representation in geometric modeling: concepts, 
 * implementation and applications", The Visual Computer, 11, 429-446, 1995.
 */

namespace e2 {

    std::ostream& operator<<(std::ostream& os, const FNodeType& type) {
        switch (type) {
            case FNodeType::MAX:
                os << "MAX";
                break;
            case FNodeType::MIN:
                os << "MIN";
                break;
            case FNodeType::NEGATION:
                os << "NEGATION";
                break;
            case FNodeType::EVALUATION:
                os << "EVALUATION";
                break;
            default:
                os << "UNKNOWN";
                break;
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const FNode& node) {
        os << "FNode(type=" << node.m_type << ")";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const FArg& arg) {
        os << "FArg(output=" << arg.outputFNode() << ", input=" << arg.inputFNode() << ")";
        return os;
    }

    FNodeIndex FObject::addFNode(const FNode& fnode) {
        m_fnodes.push_back(fnode);
        m_graphNeedsUpdate = true;
        return m_fnodes.size() - 1;
    }

    FArgIndex FObject::addFArg(const FArg& farg) {
        m_fargs.push_back(farg);
        m_graphNeedsUpdate = true;
        return m_fargs.size() - 1;
    }

    void FObject::attachEvaluator(FNodeIndex fnodeIndex, FEvaluator* evaluator) {
        m_Evaluators[fnodeIndex] = evaluator;
    }

    bool FObject::findEvaluator(FNodeIndex fnodeIndex, FEvaluator*& outEvaluator) const {
        auto it = m_Evaluators.find(fnodeIndex);
        if (it != m_Evaluators.end()) {
            outEvaluator = it->second;
            return true;
        }
        return false;
    }

    void FObject::updateGraph() {
        if (m_graphNeedsUpdate) {
            // For now, just rebuild the graph from scratch. Incremental update can come later.
            m_graph = e2::Graph(m_fnodes.size());
            for (FArgIndex i = 0; i < m_fargs.size(); ++i) {
                const auto& farg = m_fargs[i];
                // Add the farg as a link connecting the output of one function with the input of another
                // TODO: consider not adding it if either fnode is inactive.
                m_graph.addLink(farg.outputFNode(), farg.inputFNode(), i);
            }
            m_graphNeedsUpdate = false;
        }
    }

    std::ostream& operator<<(std::ostream& os, const FObject& object) {
        os << "FObject with " << object.numFNodes() << " FNodes and " << object.numFArgs() << " FArgs." << std::endl;
        for (FNodeIndex i = 0; i < object.numFNodes(); ++i) {
            os << "  FNode " << i << ": " << object.fnode(i) << std::endl;
        }
        for (FArgIndex i = 0; i < object.numFArgs(); ++i) {
            os << "  FArg " << i << ": " << object.farg(i) << std::endl;
        }
        return os;
    }


};


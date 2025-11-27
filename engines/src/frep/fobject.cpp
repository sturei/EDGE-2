#include <iostream>

#include "utils/graph.h"
#include "frep/fobject.h"

/***
 * The FObject class represents a closed subset of 3-dimensional space as the set of points P 
 * where f(P) <= 0, f being a continuous real function.
 * 
 * The key concepts of the implementation are Function, FNode and FArg.
 * 
 * Arbitrary functions may be implemented by subclassing the Function class and
 * attaching an instance of the subclassed function to an FNode. 
 * Functions may be nullary (having no arguments) or evaluated from the output of other Functions. 
 * 
 * FNodes and FArgs provide the connectivity between Functions. In general, an FNode can have multiple input arguments 
 * (represented by multiple FArgs) and its value (output) may be used as the input to multiple other FNodes.
 * Thus we obtain a directed acyclic graph of FNodes connected by FArgs.
 * 
 * This graph is represented explicitly using the Graph class attached to the FObject. The graph can rebuilt 
 * from the FNodes and FArgs whenever needed.
 * 
 * FNodes and FArgs, once added to a FObject, cannot be removed (FNodes can be deactivated though).
 * Hence, indexes into the FObject's FNode and FArg vectors remain valid for the lifetime of the FObject.
 *
 * Bibliography:
 * Pasko, Adzhiev, Sourin and Savchenko, "Function representation in geometric modeling: concepts, 
 * implementation and applications", The Visual Computer, 11, 429-446, 1995.
 */

namespace e2 {

    std::ostream& operator<<(std::ostream& os, const FNode& node) {
        os << "FNode(functionIndex=" << node.m_functionIndex << ")";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const FArg& arg) {
        os << "FArg(output=" << arg.outputFNode() << ", input=" << arg.inputFNode() << ")";
        return os;
    }

    FunctionIndex FObject::addFunction(Function* function) {
        m_functions.push_back(function);
        return m_functions.size() - 1;
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

    FObject::FObject(const std::vector<Function*>& functions, const std::vector<FNode>& fnodes, const std::vector<FArg>& fargs, FNodeIndex root) :
        m_functions(functions), m_fnodes(fnodes), m_fargs(fargs), m_rootIndex(root) {
        updateGraph();
    }

    FObject::FObject(const FObject& other)
        : m_fnodes(other.m_fnodes),
          m_fargs(other.m_fargs),
          m_rootIndex(other.m_rootIndex),
          m_graphNeedsUpdate(other.m_graphNeedsUpdate) {
        // Deep copy of functions
        for (const auto& func : other.m_functions) {
            if (func) {
                m_functions.push_back(func->clone());
            } else {
                m_functions.push_back(nullptr);
            }
        }
        // Copy the graph if it doesn't need an update
        if (!m_graphNeedsUpdate) {
            m_graph = other.m_graph;
        }
    }

    FObject::~FObject() {
        // destroy the functions.
        for (auto& function : m_functions) {
            delete function;
        }
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

    // Counts the number of input arguments to the specified fnode
    size_t arity(const FObject& object, FNodeIndex fNodeIndex) {
        if (object.graphNeedsUpdate()) {
            const_cast<FObject&>(object).updateGraph();
        }
        const Graph& graph = object.graph();
        const Graph::NodeView node = graph.node(fNodeIndex);     // indices into graph and object nodes are the same
        return node.inDegree;
    }

    // Gets the list of input fnodes to the specified fnode
    std::vector<FNodeIndex> getInputFNodes(const FObject& object, FNodeIndex fNodeIndex) {
        if (object.graphNeedsUpdate()) {
            const_cast<FObject&>(object).updateGraph();
        }
        const Graph& graph = object.graph();
        const Graph::NodeView node = graph.node(fNodeIndex);
        std::vector<FNodeIndex> inputNodes; 
        for (size_t i = 0; i < node.inDegree; ++i) {
            inputNodes.push_back(node.inLink(i).source);
        }
        return inputNodes;
    }   

    // Evaluates the fobject at the given position, starting from the specified node
    static bool evaluateNode(const FObject& fobject, FNodeIndex nodeIndex,const Vec3d& position, double& output) {
        
        // gather input values
        const auto inputNodes = getInputFNodes(fobject, nodeIndex);
        std::vector<double> inputValues;
        for (const auto& inputNodeIndex : inputNodes) {
            double inputValue = 0.0;
            if (evaluateNode(fobject, inputNodeIndex, position, inputValue)) {
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

    // Evaluates the fobject at the given position, starting from the root node
    bool FObject::evaluate(const Vec3d& position, double& output) const {
        FNodeIndex rootIndex = m_rootIndex;
        bool result = evaluateNode(*this, m_rootIndex, position, output);
        //std::cerr << "evaluate(FObject) at " << position << ": output = " << output << std::endl;
        return result;
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


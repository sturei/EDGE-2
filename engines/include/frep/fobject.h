#pragma once
#include "utils/graph.h"
#include "utils/vec3d.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace e2 {

    typedef size_t FNodeIndex;       // Index into the Object's FNode list
    typedef size_t FArgIndex;        // Index into the Object's FArg list
    typedef size_t FunctionIndex;    // Index into the Object's Function list

    class Function {
    public:
        virtual ~Function() {}
        virtual bool evaluate(const Vec3d& positionIn, const std::vector<double>& argsIn, double& valueOut) const = 0;
        virtual void print(std::ostream& os) const = 0;
    };

    class FNode {
        public:
            FNode(FunctionIndex functionIndex, bool active = true) : 
                m_functionIndex(functionIndex), m_active(active) {}
            FunctionIndex functionIndex() const { return m_functionIndex; }
            bool isActive() const { return m_active; }
            friend std::ostream& operator<<(std::ostream& os, const FNode& fnode);
        private:
            FunctionIndex m_functionIndex = -1; // index into the FObject's function list
            bool m_active = true; // whether the fnode should be evaluated or ignored
    };

    class FArg {
        public:
            FArg(FNodeIndex outputFNode, FNodeIndex inputFNode) : m_outputFNode(outputFNode), m_inputFNode(inputFNode) {}
            FNodeIndex outputFNode() const { return m_outputFNode; }
            FNodeIndex inputFNode() const { return m_inputFNode; }
            friend std::ostream& operator<<(std::ostream& os, const FArg& farg);
        private:
            FNodeIndex m_outputFNode = -1;
            FNodeIndex m_inputFNode = -1;
    };

    class FObject {
        public:
            FObject() : m_graphNeedsUpdate(false) {}
            FObject(const std::vector<Function*>& functions,
                const std::vector<FNode>& fnodes,
                const std::vector<FArg>& fargs, 
                FNodeIndex root) :
                m_functions(functions), m_fnodes(fnodes), m_fargs(fargs), m_rootIndex(root) {
                updateGraph();
            }
            ~FObject() {
                // destroy the functions.
                for (auto& function : m_functions) {
                    delete function;
                }
            }

            const std::vector<Function*> functions() const { return m_functions; }
            const std::vector<FNode>& fnodes() const { return m_fnodes; }
            const std::vector<FArg>& fargs() const { return m_fargs; }
            size_t numFunctions() const { return m_functions.size(); }
            size_t numFNodes() const { return m_fnodes.size(); }
            size_t numFArgs() const { return m_fargs.size(); }

            FunctionIndex addFunction(Function* function);
            Function* function(FunctionIndex index) const { return m_functions[index]; }
            FNodeIndex addFNode(const FNode& fnode);
            const FNode& fnode(FNodeIndex index) const { return m_fnodes[index]; }
            FArgIndex addFArg(const FArg& farg);
            const FArg& farg(FArgIndex index) const { return m_fargs[index]; }
            
            void setRootIndex(FNodeIndex index) { m_rootIndex = index; }
            FNodeIndex rootIndex() const { return m_rootIndex; }
            
            const e2::Graph& graph() const { return m_graph; }
            bool graphNeedsUpdate() const { return m_graphNeedsUpdate; }    
            void updateGraph();

            bool evaluate(const Vec3d& position, double& output) const;

            friend std::ostream& operator<<(std::ostream& os, const FObject& object);
        private:
            std::vector<FNode> m_fnodes;           // all the fnodes in the object
            std::vector<FArg> m_fargs;             // all the fargs in the object
            std::vector<Function*> m_functions;    // all the functions used by the object
            FNodeIndex m_rootIndex = -1;           // the root fnode of the object
            e2::Graph m_graph;                     // graph representing the connectivity of fnodes via fargs
            bool m_graphNeedsUpdate = true;        // whether the graph needs to be rebuilt from the fnodes and fargs
    };

    // exported to make them visible to tests
    size_t arity(const FObject& object, FNodeIndex nodeIndex);
    std::vector<FNodeIndex> getInputFNodes(const FObject& object, FNodeIndex nodeIndex);

};


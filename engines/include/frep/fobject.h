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
    typedef size_t FEvaluatorIndex;  // Index into the Object's FEvaluator list

    class FEvaluator {
    public:
        virtual ~FEvaluator() {}
        virtual bool evaluate(const Vec3d& positionIn, const std::vector<double>& argsIn, double& valueOut) const = 0;
        virtual void print(std::ostream& os) const = 0;
    };

    class FNode {
        public:
            FNode(FEvaluatorIndex evaluatorIndex, bool active = true) : 
                m_evaluatorIndex(evaluatorIndex), m_active(active) {}
            FEvaluatorIndex evaluatorIndex() const { return m_evaluatorIndex; }
            bool isActive() const { return m_active; }
            friend std::ostream& operator<<(std::ostream& os, const FNode& fnode);
        private:
            FEvaluatorIndex m_evaluatorIndex = -1; // index into the FObject's evaluator list
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
            FObject(const std::vector<FEvaluator*>& evaluators,
                const std::vector<FNode>& fnodes,
                const std::vector<FArg>& fargs, 
                FNodeIndex root) :
                m_evaluators(evaluators), m_fnodes(fnodes), m_fargs(fargs), m_rootIndex(root) {
                updateGraph();
            }
            ~FObject() {
                // destroy the evaluators.
                for (auto& evaluator : m_evaluators) {
                    delete evaluator;
                }
            }

            const std::vector<FEvaluator*> evaluators() const { return m_evaluators; }
            const std::vector<FNode>& fnodes() const { return m_fnodes; }
            const std::vector<FArg>& fargs() const { return m_fargs; }
            size_t numEvaluators() const { return m_evaluators.size(); }
            size_t numFNodes() const { return m_fnodes.size(); }
            size_t numFArgs() const { return m_fargs.size(); }

            FEvaluatorIndex addEvaluator(FEvaluator* evaluator);
            FEvaluator* evaluator(FEvaluatorIndex index) const { return m_evaluators[index]; }
            FNodeIndex addFNode(const FNode& fnode);
            const FNode& fnode(FNodeIndex index) const { return m_fnodes[index]; }
            FArgIndex addFArg(const FArg& farg);
            const FArg& farg(FArgIndex index) const { return m_fargs[index]; }
            
            void setRootIndex(FNodeIndex index) { m_rootIndex = index; }
            FNodeIndex rootIndex() const { return m_rootIndex; }
            
            const e2::Graph& graph() const { return m_graph; }
            bool graphNeedsUpdate() const { return m_graphNeedsUpdate; }    
            void updateGraph();

            friend std::ostream& operator<<(std::ostream& os, const FObject& object);
        private:
            std::vector<FNode> m_fnodes;           // all the fnodes in the object
            std::vector<FArg> m_fargs;             // all the fargs in the object
            std::vector<FEvaluator*> m_evaluators; // all the evaluators used by the object
            FNodeIndex m_rootIndex = -1;           // the root fnode of the object
            e2::Graph m_graph;                     // graph representing the connectivity of fnodes via fargs
            bool m_graphNeedsUpdate = true;        // whether the graph needs to be rebuilt from the fnodes and fargs
    };
};


#pragma once
#include "utils/graph.h"
#include "utils/vec3d.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>


namespace e2 {

    typedef size_t FNodeIndex;       // Index into the Object's FNode vector
    typedef size_t FArgIndex;        // Index into the Object's FArg vector

    enum class FNodeType {
        MAX,                // returns the maximum of its n arguments (implements INTERSECTION)
        MIN,                // returns the minimum of its n arguments (implements UNION)
        NEGATION,           // returns the negation of its single argument (implements COMPLEMENT)
        EVALUATION          // returns its value, based on a custom evaluator
    };

    std::ostream& operator<<(std::ostream& os, const FNodeType& type);

    class FNode {
        public:
            FNode() {}
            explicit FNode(FNodeType type, bool active = true) : m_type(type), m_active(active) {}
            FNodeType type() const { return m_type; }
            bool isActive() const { return m_active; }
            friend std::ostream& operator<<(std::ostream& os, const FNode& fnode);
        private:
            FNodeType m_type;
            bool m_active = true; // whether the fnode should be evaluated or ignored
    };

    class FArg {
        public:
            FArg() {}
            FArg(FNodeIndex outputFNode, FNodeIndex inputFNode) : m_outputFNode(outputFNode), m_inputFNode(inputFNode) {}
            FNodeIndex outputFNode() const { return m_outputFNode; }
            FNodeIndex inputFNode() const { return m_inputFNode; }
            friend std::ostream& operator<<(std::ostream& os, const FArg& farg);
        private:
            FNodeIndex m_outputFNode = -1;
            FNodeIndex m_inputFNode = -1;
    };

    class FEvaluator {
        public:
            virtual ~FEvaluator() {};
            virtual bool evaluate(const Vec3d& position, const std::vector<double>& inputs, double& output) const = 0;
            virtual void print(std::ostream& os) const = 0;
            friend std::ostream& operator<<(std::ostream& os, const FEvaluator& e);
        };

    class FObject {
        public:
            FObject() : m_graphNeedsUpdate(false) {}
            FObject(const std::vector<FNode>& fnodes, const std::vector<FArg>& fargs = {}) : m_fnodes(fnodes), m_fargs(fargs) {
                updateGraph();
            }
            ~FObject() {
                // destroy the evaluators.
                for (auto& pair : m_Evaluators) {
                    delete pair.second;
                }
            }

            const std::vector<FNode>& fnodes() const { return m_fnodes; }
            const std::vector<FArg>& fargs() const { return m_fargs; }
            FNodeIndex addFNode(const FNode& fnode);
            const FNode& fnode(FNodeIndex index) const { return m_fnodes[index]; }
            const size_t numFNodes() const { return m_fnodes.size(); }
            FArgIndex addFArg(const FArg& farg);
            const FArg& farg(FArgIndex index) const { return m_fargs[index]; }
            const size_t numFArgs() const { return m_fargs.size(); }

            const e2::Graph& graph() const { return m_graph; }
            bool graphNeedsUpdate() const { return m_graphNeedsUpdate; }    
            void updateGraph();

            void attachEvaluator(FNodeIndex fnodeIndex, FEvaluator* evaluator);   // object takes ownership of evaluator pointer
            bool findEvaluator(FNodeIndex fnodeIndex, FEvaluator*& outEvaluator) const;

            friend std::ostream& operator<<(std::ostream& os, const FObject& object);
        private:
            std::vector<FNode> m_fnodes; // all the fnodes in the object
            std::vector<FArg> m_fargs;   // all the fargs in the object
            FNodeIndex m_rootFNode;           // the root fnode of the object
            std::map<FNodeIndex, FEvaluator*> m_Evaluators; // evaluators attached to FNodes
            e2::Graph m_graph; // graph representing the connectivity of fnodes via fargs
            bool m_graphNeedsUpdate = true; // whether the graph needs to be rebuilt from the fnodes and fargs
    };
};


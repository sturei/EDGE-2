
#include "frep/fnavigate.h"

namespace e2 {
    
    size_t arity(const FObject& object, FNodeIndex fNodeIndex) {
        if (object.graphNeedsUpdate()) {
            const_cast<FObject&>(object).updateGraph();
        }
        const Graph& graph = object.graph();
        const Graph::NodeView node = graph.node(fNodeIndex);     // indices into graph and object nodes are the same
        return node.inDegree;
    }

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

};  


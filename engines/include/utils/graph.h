#pragma once
#include <vector>

/**
 * The Graph class represents a directed labelled multigraph.
 * Implementation notes:
 * Each element nodes[i] represents a vertex in the graph, referenced by its index i into a vector.
 * Each nodes has two vectors of links, outLinks and inLinks, which store indexes of target and source nodes respectively.
 * Parallel links (i.e. multiple links connecting same source and target node) are permitted
 * An link may connect a node to itself
 * Adding an link to a non-existent node is a no-op
 * Each node, each link and the graph itself has an associated property, opaque to the graph, represented as an index which it is
 * assumed can be used by client code to look up the value of the property elsewhere.
 * An index of -1 indicates no property.
 * Future extensions:
 * addNode
 * clearNode
 * swapNodes/links (utilities for remove)
 * removeNode
 * removeLink
 * X(X&) and operator=
 * maybe add iterators
 * Note that indexes and iterators will be unstable over structural modifications (bar adding vertices/edges), owing to the use of vectors to store vertices and edges.
 * Thinking about cache usage:
 * My 2017 iMac sysctl -a | grep "l.*cachesize" gives:
 *   hw.l1icachesize: 32768
 *   hw.l1dcachesize: 32768
 *   hw.l2cachesize: 262144
 *   hw.l3cachesize: 8388608
 * At time of writing, a NodeImpl is 96 bytes. Node properties add another 8 bytes per node. So a node with properties is 104 bytes.
 * A link (out-link and in-link pair) is 16 bytes. Link properties add another 16 bytes per link pair. So a link with properties is 32 bytes.
 * L1d cache is 32kB, which is 32768/104 = 340 NodeImpl objects.
 * L2 cache is 256kB, which is 262144/104 = 2520 NodeImpl objects.
 * L3 cache is 8MB, which is 8388608/104 = 80640 NodeImpl objects.
 * A graph with 100 nodes and 200 links is 100*104 + 200*32 = 16kB (not counting the graph structure itself), which fits in L1 cache.
 * A graph with 1000 nodes and 2000 links is 164kB (not counting the graph structure itself), which fits in L2 cache but not L1 cache.
 */

namespace e2 {
    class Graph {
    public:

        Graph() {}

        Graph(size_t numNodes, size_t graphProperty = 0, const std::vector<size_t>& nodeProperties = Graph::nullNodeProperties)
            : m_nodes(numNodes), m_graphProperty(graphProperty), m_nodeProperties(nodeProperties) {
            if (m_nodeProperties.empty()) {
                m_nodeProperties = std::vector<size_t>(numNodes, 0);
            }
        }

        void addLink(size_t u, size_t v, size_t linkProperty = 0) {
            if (u < numNodes() && v < numNodes()) {
                m_nodes[u].outLinks.push_back(v);
                m_nodes[u].outLinkProperties.push_back(linkProperty);
                m_nodes[v].inLinks.push_back(u);
                m_nodes[v].inLinkProperties.push_back(linkProperty);
            }
        }

        /**
         * Accessors provdiding read-only views of the graph's structure and properties without returning references to any underlying data structures.
        */
        struct LinkView {
            size_t index; // index of this link in the in or out links of its node
            size_t source;
            size_t target;
            size_t linkProperty;
        };

        struct NodeView {
            public:
                size_t index; // index of this node in the graph
                size_t outDegree;
                size_t inDegree;
                size_t nodeProperty;
                LinkView outLink(size_t i) const {
                    return LinkView{ i, index, outLinks[i], outLinkProperties[i] };
                }
                LinkView inLink(size_t i) const {
                    return LinkView{ i, inLinks[i], index, inLinkProperties[i] };
                }
            private:
                friend class e2::Graph;
                NodeView( size_t idx,
                            const std::vector<size_t>& outLinks,
                            const std::vector<size_t>& outLinkProperties,
                            size_t outD,
                            const std::vector<size_t>& inLinks,
                            const std::vector<size_t>& inLinkProperties,
                            size_t inD,
                            size_t vP)
                    : index(idx), outLinks(outLinks), outLinkProperties(outLinkProperties), outDegree(outD),
                    inLinks(inLinks), inLinkProperties(inLinkProperties), inDegree(inD), nodeProperty(vP) {}
                const std::vector<size_t>& outLinks;
                const std::vector<size_t>& outLinkProperties;
                const std::vector<size_t>& inLinks;
                const std::vector<size_t>& inLinkProperties;
        };

        size_t numNodes() const {
            return m_nodes.size();
        }

        NodeView node(size_t u) const {
            return NodeView{ u, 
                m_nodes[u].outLinks, m_nodes[u].outLinkProperties, m_nodes[u].outLinks.size(),
                m_nodes[u].inLinks, m_nodes[u].inLinkProperties, m_nodes[u].inLinks.size(),
                m_nodeProperties[u] };
        }

        size_t graphProperty() const {
            return m_graphProperty;
        }

        friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
            os << "Graph property: " << g.graphProperty() << "\n";
            os << "Nodes: " << g.numNodes() << "\n";
            for (size_t u = 0; u < g.numNodes(); ++u) {
                auto n = g.node(u);
                os << "  Node " << n.index << " (property: " << n.nodeProperty << "):\n";
                os << "    OutLinks (" << n.outDegree << "):";
                for (size_t i = 0; i < n.outDegree; ++i) {
                    auto e = n.outLink(i);
                    os << " [" << e.target << " prop:" << e.linkProperty << "]";
                }
                os << "\n";
                os << "    InLinks (" << n.inDegree << "):";
                for (size_t i = 0; i < n.inDegree; ++i) {
                    auto e = n.inLink(i);
                    os << " [" << e.source << " prop:" << e.linkProperty << "]";
                }
                os << "\n";
            }
            return os;
        }

    private:

        inline static std::vector<size_t> nullNodeProperties;  // TODO: make this a static member of the class and define it in the graph.cpp file. Inline variables are C++17 btw

        struct NodeImpl {
            std::vector<size_t> outLinks;
            std::vector<size_t> inLinks;
            std::vector<size_t> outLinkProperties;
            std::vector<size_t> inLinkProperties;
        };
        std::vector<NodeImpl> m_nodes;
        std::vector<size_t> m_nodeProperties;
        size_t m_graphProperty = 0;
    };
};

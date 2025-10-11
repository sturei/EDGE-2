#pragma once
#include <vector>

/**
 * The Graph class represents a directed labelled multigraph.
 * Implementation notes:
 * Each element vertices[i] represents a vertex in the graph, referenced by its index i into a vector.
 * Each vertex has two vectors of edges, outEdges and inEdges, which store indexes of target and source vertices respectively.
 * Parallel edges (i.e. multiple edges connecting same source and target vertex) are permitted
 * An edge may connect a vertex to itself
 * Adding an edge to a non-existent vertex is a no-op
 * Each vertex, each edge and the graph itself has an associated property, opaque to the graph, represented as an index which it is 
 * assumed can be used by client code to look up the value of the property elsewhere. 
 * An index of -1 indicates no property.
 * Future extensions:
 * addVertex
 * clearVertex
 * swapVertices/edges (utilities for remove)
 * removeVertex
 * removeEdge
 * X(X&) and operator=
 * maybe add iterators
 * Note that indexes and iterators will be unstable over structural modifications (bar adding vertices/edges), owing to the use of vectors to store vertices and edges.
 * Thinking about cache usage:
 * My 2017 iMac sysctl -a | grep "l.*cachesize" gives:
 *   hw.l1icachesize: 32768
 *   hw.l1dcachesize: 32768
 *   hw.l2cachesize: 262144
 *   hw.l3cachesize: 8388608
 * At time of writing, a VertexImpl is 96 bytes. Vertex properties add another 8 bytes per vertex. So a vertex with properties is 104 bytes.
 * An edge (out-edge and in-edge pair) is 16 bytes. Edge properties add another 16 bytes per edge pair. So an edge with properties is 32 bytes.
 * L1d cache is 32kB, which is 32768/104 = 340 VertexImpl objects.
 * L2 cache is 256kB, which is 262144/104 = 2520 VertexImpl objects.
 * L3 cache is 8MB, which is 8388608/104 = 80640 VertexImpl objects.
 * A graph with 100 vertices and 200 edges is 100*104 + 200*32 = 16kB (not counting the graph structure itself), which fits in L1 cache.
 * A graph with 1000 vertices and 2000 edges is 164kB (not counting the graph structure itself), which fits in L2 cache but not L1 cache.    
 */

namespace e2 {
    class Graph {
    public:

        Graph() {}
        
        Graph(size_t numVertices, size_t graphProperty = -1, const std::vector<size_t>& vertexProperties = Graph::nullVertexProperties)
            : m_vertices(numVertices), m_graphProperty(graphProperty), m_vertexProperties(vertexProperties) {
            if (m_vertexProperties.empty()) {
                m_vertexProperties = std::vector<size_t>(numVertices, -1);
            }
        }

        void addEdge(size_t u, size_t v, size_t edgeProperty = -1) {
            if (u < numVertices() && v < numVertices()) {
                m_vertices[u].outEdges.push_back(v);
                m_vertices[u].outEdgeProperties.push_back(edgeProperty);
                m_vertices[v].inEdges.push_back(u);
                m_vertices[v].inEdgeProperties.push_back(edgeProperty);
            }
        }

        /**
         * Accessors provdiding read-only views of the graph's structure and properties without returning references to any underlying data structures.
        */
        struct EdgeView {
            size_t index; // index of this edge in the in or out edges of its vertex
            size_t source;
            size_t target;
            size_t edgeProperty;
        };

        struct VertexView {
            public:
                size_t index; // index of this vertex in the graph
                size_t outDegree;
                size_t inDegree;
                size_t vertexProperty;
                EdgeView outEdge(size_t i) const {
                    return EdgeView{ i, index, outEdges[i], outEdgeProperties[i] };
                }
                EdgeView inEdge(size_t i) const {
                    return EdgeView{ i, inEdges[i], index, inEdgeProperties[i] };
                }
            private:
                friend class e2::Graph;
                VertexView( size_t idx,
                            const std::vector<size_t>& outE,
                            const std::vector<size_t>& outEP,
                            size_t outD,
                            const std::vector<size_t>& inE,
                            const std::vector<size_t>& inEP,
                            size_t inD,
                            size_t vP)
                    : index(idx), outEdges(outE), outEdgeProperties(outEP), outDegree(outD),
                    inEdges(inE), inEdgeProperties(inEP), inDegree(inD), vertexProperty(vP) {}
                const std::vector<size_t>& outEdges;
                const std::vector<size_t>& outEdgeProperties;
                const std::vector<size_t>& inEdges;
                const std::vector<size_t>& inEdgeProperties;
        };

        size_t numVertices() const {
            return m_vertices.size();
        }

        VertexView vertex(size_t u) const {
            return VertexView{ u, 
                m_vertices[u].outEdges, m_vertices[u].outEdgeProperties, m_vertices[u].outEdges.size(),
                m_vertices[u].inEdges, m_vertices[u].inEdgeProperties, m_vertices[u].inEdges.size(),
                m_vertexProperties[u] };
        }

        size_t graphProperty() const {
            return m_graphProperty;
        }

    private:

        inline static std::vector<size_t> nullVertexProperties;  // TODO: make this a static member of the class and define it in the graph.cpp file. Inline variables are C++17 btw

        struct VertexImpl {
            std::vector<size_t> outEdges;
            std::vector<size_t> inEdges;
            std::vector<size_t> outEdgeProperties;
            std::vector<size_t> inEdgeProperties;
        };
        std::vector<VertexImpl> m_vertices;
        std::vector<size_t> m_vertexProperties;
        size_t m_graphProperty = -1;
    };
};

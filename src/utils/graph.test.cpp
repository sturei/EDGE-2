#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Contains;

#include "utils/graph.h"

// Define a simple class to represent a map of cities connected by roads
struct Map
{
    std::string name;
    std::vector<std::string> cities;
    std::vector<std::string> roads;
    e2::Graph connections;

    // Some (not optimized) utilities to help with testing
    size_t cityIndex(const std::string& city) const {
        for (size_t i = 0; i < cities.size(); ++i) {
            if (cities[i] == city) return i;
        }
        return -1;
    }
    size_t roadIndex(const std::string& road) const {
        for (size_t i = 0; i < roads.size(); ++i) { 
            if (roads[i] == road) return i;
        }
        return -1;
    }
    void connect(const std::string& sourceCity, const std::string& targetCity, const std::string& road) {
        size_t u = cityIndex(sourceCity);
        size_t v = cityIndex(targetCity);
        size_t p = roadIndex(road);
        connections.addEdge(u, v, p);
    }
    std::vector<std::string> getRoadsBetween(const std::string& sourceCity, const std::string& targetCity) const {
        size_t u = cityIndex(sourceCity);
        size_t v = cityIndex(targetCity);
        const auto& sourceVertex = connections.vertex(u);

        std::vector<std::string> roadsOut;
        for (size_t i = 0; i < sourceVertex.outDegree; ++i) {
            const auto& outEdge = sourceVertex.outEdge(i);
            if (outEdge.target == v) {
              roadsOut.push_back(roads[outEdge.edgeProperty]);
            }
        }
        return roadsOut;
    }
};

// Define a simple atlas containing some maps
struct Atlas
{
    std::vector<Map> maps;
    Atlas() : maps{
        {
            "symbols",
            {"A", "B", "C", "D", "E"},
            {"AB", "AC", "BC", "CA", "CD", "DD"},
            e2::Graph(5,
                0,
                {0, 1, 2, 3, 4})
        },
        {
            "North America",
            {"Vancouver", "Seattle", "Portland"},
            {"VS", "VP", "SP"},
            e2::Graph(3,
                1,
                {0, 1, 2})
        }
    } {
        // Add edges and properties for first map directly
        maps[0].connections.addEdge(0, 1, 0); // A -> B
        maps[0].connections.addEdge(0, 2, 1); // A -> C
        maps[0].connections.addEdge(1, 2, 2); // B -> C
        maps[0].connections.addEdge(2, 0, 3); // C -> A
        maps[0].connections.addEdge(2, 3, 4); // C -> D
        maps[0].connections.addEdge(3, 3, 5); // D -> D

        // Add edges for second map using the utilities
        maps[1].connect("Vancouver", "Seattle", "VS");
        maps[1].connect("Vancouver", "Portland", "VP");
        maps[1].connect("Seattle", "Portland", "SP");
    }    
};

// a few not very efficient utilities to help with testing. These might be helped by adding iterators to the graph class in the future.
std::vector<size_t> getOutEdges(const e2::Graph& g, size_t u) {
    std::vector<size_t> outEdges;
    const auto& vertex = g.vertex(u);
    for (size_t i = 0; i < vertex.outDegree; ++i) {
        outEdges.push_back(vertex.outEdge(i).target);
    }
    return outEdges;
} 
std::vector<size_t> getInEdges(const e2::Graph& g, size_t u) {
    std::vector<size_t> inEdges;
    const auto& vertex = g.vertex(u);
    for (size_t i = 0; i < vertex.inDegree; ++i) {
        inEdges.push_back(vertex.inEdge(i).source);
    }
    return inEdges;
}
// end of utilities

class GraphTest : public ::testing::Test {
 protected:
  void SetUp() override {

    // "graph" is for testing basic graph functionality (without properties)
    graph = new e2::Graph(5);
    graph->addEdge(0, 1);
    graph->addEdge(0, 2);
    graph->addEdge(1, 2);
    graph->addEdge(2, 0);
    graph->addEdge(2, 3);
    graph->addEdge(3, 3);

    // "atlas" is for testing property functionality
    atlas = new Atlas();    
    }

    void TearDown() override {
      delete graph;
      delete atlas;
    }
    e2::Graph* graph;
    Atlas* atlas;
};


TEST_F(GraphTest, Size) {
  EXPECT_EQ(graph->numVertices(), 5);
}
         
TEST_F(GraphTest, OutEdges) {
  std::vector<size_t> outEdges0 = getOutEdges(*graph, 0);
  EXPECT_EQ(outEdges0.size(), 2);
  EXPECT_THAT(outEdges0, Contains(1));
  EXPECT_THAT(outEdges0, Contains(2));

  std::vector<size_t> outEdges2 = getOutEdges(*graph, 2);
  EXPECT_EQ(outEdges2.size(), 2);
  EXPECT_THAT(outEdges2, Contains(0));
  EXPECT_THAT(outEdges2, Contains(3));

  std::vector<size_t> outEdges3 = getOutEdges(*graph, 3);
  EXPECT_EQ(outEdges3.size(), 1);
  EXPECT_THAT(outEdges3, Contains(3));

  std::vector<size_t> outEdges4 = getOutEdges(*graph, 4);
  EXPECT_EQ(outEdges4.size(), 0);

}

TEST_F(GraphTest, InEdges) {
  std::vector<size_t> inEdges0 = getInEdges(*graph, 0);
  EXPECT_EQ(inEdges0.size(), 1);
  EXPECT_THAT(inEdges0, Contains(2));

  std::vector<size_t> inEdges2 = getInEdges(*graph, 2);
  EXPECT_EQ(inEdges2.size(), 2);
  EXPECT_THAT(inEdges2, Contains(0));
  EXPECT_THAT(inEdges2, Contains(1));

  std::vector<size_t> inEdges3 = getInEdges(*graph, 3);
  EXPECT_EQ(inEdges3.size(), 2);
  EXPECT_THAT(inEdges3, Contains(2));
  EXPECT_THAT(inEdges3, Contains(3));

  std::vector<size_t> inEdges4 = getInEdges(*graph, 4);
  EXPECT_EQ(inEdges4.size(), 0);

}

TEST_F(GraphTest, AddEdgeOutOfBounds) {
  // Adding an edge with out-of-bounds vertices should not change the graph
  graph->addEdge(5, 1); // u is out of bounds
  graph->addEdge(1, 5); // v is out of bounds
  graph->addEdge(6, 7); // both u and v are out of bounds

  EXPECT_EQ(graph->numVertices(), 5);
  EXPECT_EQ(graph->vertex(1).outDegree, 1);
  EXPECT_EQ(graph->vertex(0).outDegree, 2);
}

TEST_F(GraphTest, PropertyValues) {
  // Check some specific property values in the first map of the atlas
  const Map& m1 = atlas->maps[0];
  const e2::Graph& g1 = m1.connections;
  EXPECT_EQ(g1.graphProperty(), 0);
  EXPECT_EQ(m1.cities[g1.vertex(0).vertexProperty], "A");
  EXPECT_EQ(m1.cities[g1.vertex(1).vertexProperty], "B");
  EXPECT_EQ(m1.roads[g1.vertex(0).outEdge(0).edgeProperty], "AB");
  EXPECT_EQ(m1.roads[g1.vertex(0).outEdge(1).edgeProperty], "AC");
  EXPECT_EQ(m1.roads[g1.vertex(2).outEdge(1).edgeProperty], "CD");
}

TEST_F(GraphTest, Accessors) {
  // use the accessors directly to get the property values in the first map of the atlas
  const Map& m1 = atlas->maps[0];
  const e2::Graph& g1 = m1.connections;
  EXPECT_EQ(g1.graphProperty(), 0);
  EXPECT_EQ(m1.cities[g1.vertex(0).vertexProperty], "A");
  EXPECT_EQ(m1.cities[g1.vertex(1).vertexProperty], "B");
  EXPECT_EQ(m1.roads[g1.vertex(0).outEdge(0).edgeProperty], "AB");
  EXPECT_EQ(m1.roads[g1.vertex(0).outEdge(1).edgeProperty], "AC");
  EXPECT_EQ(m1.roads[g1.vertex(2).outEdge(1).edgeProperty], "CD");

  // use the utilities to get the property values in the second map of the atlas
  const Map& m2 = atlas->maps[1];

  std::vector<std::string> roadsVS = m2.getRoadsBetween("Vancouver", "Seattle");
  EXPECT_EQ(roadsVS.size(), 1);
  EXPECT_THAT(roadsVS, Contains("VS"));

  std::vector<std::string> roadsVP = m2.getRoadsBetween("Vancouver", "Portland");
  EXPECT_EQ(roadsVP.size(), 1);
  EXPECT_THAT(roadsVP, Contains("VP"));

  std::vector<std::string> roadsSP = m2.getRoadsBetween("Seattle", "Portland");
  EXPECT_EQ(roadsSP.size(), 1);
  EXPECT_THAT(roadsSP, Contains("SP"));

  }

  

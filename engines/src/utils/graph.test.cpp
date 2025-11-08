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
        connections.addLink(u, v, p);
    }
    std::vector<std::string> getRoadsBetween(const std::string& sourceCity, const std::string& targetCity) const {
        size_t u = cityIndex(sourceCity);
        size_t v = cityIndex(targetCity);
        const auto& sourceNode = connections.node(u);

        std::vector<std::string> roadsOut;
        for (size_t i = 0; i < sourceNode.outDegree; ++i) {
            const auto& outLink = sourceNode.outLink(i);
            if (outLink.target == v) {
              roadsOut.push_back(roads[outLink.linkProperty]);
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
        maps[0].connections.addLink(0, 1, 0); // A -> B
        maps[0].connections.addLink(0, 2, 1); // A -> C
        maps[0].connections.addLink(1, 2, 2); // B -> C
        maps[0].connections.addLink(2, 0, 3); // C -> A
        maps[0].connections.addLink(2, 3, 4); // C -> D
        maps[0].connections.addLink(3, 3, 5); // D -> D

        // Add links for second map using the utilities
        maps[1].connect("Vancouver", "Seattle", "VS");
        maps[1].connect("Vancouver", "Portland", "VP");
        maps[1].connect("Seattle", "Portland", "SP");
    }    
};

// a few not very efficient utilities to help with testing. These might be helped by adding iterators to the graph class in the future.
std::vector<size_t> getOutLinks(const e2::Graph& g, size_t u) {
    std::vector<size_t> outLinks;
    const auto& node = g.node(u);
    for (size_t i = 0; i < node.outDegree; ++i) {
        outLinks.push_back(node.outLink(i).target);
    }
    return outLinks;
} 
std::vector<size_t> getInLinks(const e2::Graph& g, size_t u) {
    std::vector<size_t> inLinks;
    const auto& node = g.node(u);
    for (size_t i = 0; i < node.inDegree; ++i) {
        inLinks.push_back(node.inLink(i).source);
    }
    return inLinks;
}
// end of utilities

class GraphTest : public ::testing::Test {
 protected:
  void SetUp() override {

      // "graph" is for testing basic graph functionality (without properties)
      graph = new e2::Graph(5);
      graph->addLink(0, 1);
      graph->addLink(0, 2);
      graph->addLink(1, 2);
      graph->addLink(2, 0);
      graph->addLink(2, 3);
      graph->addLink(3, 3);

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

TEST_F(GraphTest, DefaultConstructor) {
  e2::Graph g;
  EXPECT_EQ(g.numNodes(), 0);
} 

TEST_F(GraphTest, ParameterizedConstructor) {
  e2::Graph g1(3);
  EXPECT_EQ(g1.numNodes(), 3);
  EXPECT_EQ(g1.graphProperty(), 0);
  EXPECT_EQ(g1.node(0).nodeProperty, 0);
  EXPECT_EQ(g1.node(1).nodeProperty, 0);
  EXPECT_EQ(g1.node(2).nodeProperty, 0);

  size_t graphProperty = 42;
  std::vector<size_t> nodeProperties = { 5, 12, 13 };
  e2::Graph g2(3, graphProperty, nodeProperties);
  EXPECT_EQ(g2.numNodes(), 3);
  EXPECT_EQ(g2.graphProperty(), graphProperty);
  EXPECT_EQ(g2.node(0).nodeProperty, 5);
  EXPECT_EQ(g2.node(1).nodeProperty, 12);
  EXPECT_EQ(g2.node(2).nodeProperty, 13);

} 

TEST_F(GraphTest, Size) {
  EXPECT_EQ(graph->numNodes(), 5);
}

TEST_F(GraphTest, OutLinks) {
  std::vector<size_t> outLinks0 = getOutLinks(*graph, 0);
  EXPECT_EQ(outLinks0.size(), 2);
  EXPECT_THAT(outLinks0, Contains(1));
  EXPECT_THAT(outLinks0, Contains(2));

  std::vector<size_t> outLinks2 = getOutLinks(*graph, 2);
  EXPECT_EQ(outLinks2.size(), 2);
  EXPECT_THAT(outLinks2, Contains(0));
  EXPECT_THAT(outLinks2, Contains(3));

  std::vector<size_t> outLinks3 = getOutLinks(*graph, 3);
  EXPECT_EQ(outLinks3.size(), 1);
  EXPECT_THAT(outLinks3, Contains(3));

  std::vector<size_t> outLinks4 = getOutLinks(*graph, 4);
  EXPECT_EQ(outLinks4.size(), 0);

}

TEST_F(GraphTest, InLinks) {
  std::vector<size_t> inLinks0 = getInLinks(*graph, 0);
  EXPECT_EQ(inLinks0.size(), 1);
  EXPECT_THAT(inLinks0, Contains(2));

  std::vector<size_t> inLinks2 = getInLinks(*graph, 2);
  EXPECT_EQ(inLinks2.size(), 2);
  EXPECT_THAT(inLinks2, Contains(0));
  EXPECT_THAT(inLinks2, Contains(1));

  std::vector<size_t> inLinks3 = getInLinks(*graph, 3);
  EXPECT_EQ(inLinks3.size(), 2);
  EXPECT_THAT(inLinks3, Contains(2));
  EXPECT_THAT(inLinks3, Contains(3));

  std::vector<size_t> inLinks4 = getInLinks(*graph, 4);
  EXPECT_EQ(inLinks4.size(), 0);

}

TEST_F(GraphTest, AddLinkOutOfBounds) {
  // Adding a link with out-of-bounds vertices should not change the graph
  graph->addLink(5, 1); // u is out of bounds
  graph->addLink(1, 5); // v is out of bounds
  graph->addLink(6, 7); // both u and v are out of bounds

  EXPECT_EQ(graph->numNodes(), 5);
  EXPECT_EQ(graph->node(1).outDegree, 1);
  EXPECT_EQ(graph->node(0).outDegree, 2);
}

TEST_F(GraphTest, PropertyValues) {
  // Check some specific property values in the first map of the atlas
  const Map& m1 = atlas->maps[0];
  const e2::Graph& g1 = m1.connections;
  EXPECT_EQ(g1.graphProperty(), 0);
  EXPECT_EQ(m1.cities[g1.node(0).nodeProperty], "A");
  EXPECT_EQ(m1.cities[g1.node(1).nodeProperty], "B");
  EXPECT_EQ(m1.roads[g1.node(0).outLink(0).linkProperty], "AB");
  EXPECT_EQ(m1.roads[g1.node(0).outLink(1).linkProperty], "AC");
  EXPECT_EQ(m1.roads[g1.node(2).outLink(1).linkProperty], "CD");
}

TEST_F(GraphTest, Accessors) {
  // use the accessors directly to get the property values in the first map of the atlas
  const Map& m1 = atlas->maps[0];
  const e2::Graph& g1 = m1.connections;
  EXPECT_EQ(g1.graphProperty(), 0);
  EXPECT_EQ(m1.cities[g1.node(0).nodeProperty], "A");
  EXPECT_EQ(m1.cities[g1.node(1).nodeProperty], "B");
  EXPECT_EQ(m1.roads[g1.node(0).outLink(0).linkProperty], "AB");
  EXPECT_EQ(m1.roads[g1.node(0).outLink(1).linkProperty], "AC");
  EXPECT_EQ(m1.roads[g1.node(2).outLink(1).linkProperty], "CD");

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

  TEST_F(GraphTest, StreamOutputOperator) {
      // Just a basic test to check that the Graph can be output to a stream.
      std::ostringstream oss;
      oss << *graph;
      std::string graphStr = oss.str();

      // Check that the output string contains some expected substrings
      EXPECT_NE(graphStr.find("Graph property: 0"), std::string::npos);
      EXPECT_NE(graphStr.find("Nodes: 5"), std::string::npos);
      EXPECT_NE(graphStr.find("Node 0 (property: 0):"), std::string::npos);
      EXPECT_NE(graphStr.find("Node 3 (property: 0):"), std::string::npos);
  }

  

#include "core-TestFramework\TestFramework.h"
#include "core\Graph.h"
#include "core\GraphDijkstra.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   struct MockNode {};
   struct MockEdge;

   typedef Graph<MockNode, MockEdge> MockGraph;

   struct MockEdge
   {
      MockGraph::Index endNodeIdx;
      int cost;

      MockEdge(MockGraph::Index _endNodeIdx, int _cost)
         : endNodeIdx(_endNodeIdx) 
         , cost(_cost)
      {}

      operator MockGraph::Index() const {return endNodeIdx;}

      int getEdgeCost() const {return cost;}
   };


}  // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(GraphDijkstra, simpleGraph)
{
   MockGraph graph;
   graph.addNode(MockNode());
   graph.addNode(MockNode());
   graph.addNode(MockNode());
   graph.addNode(MockNode());
   graph.connect(0, MockEdge(1, 1));
   graph.connect(0, MockEdge(2, 2));
   graph.connect(2, MockEdge(1, 5));
   graph.connect(1, MockEdge(3, 3));

   MockGraph result;
   GraphDijkstra(result, graph, 0);

   CPPUNIT_ASSERT_EQUAL((unsigned int)3, result.getEdgesCount());

   CPPUNIT_ASSERT(result.getEdgeIdx(0, 1) != MockGraph::InvalidIndex);
   CPPUNIT_ASSERT_EQUAL(1, result.getEdge(result.getEdgeIdx(0, 1)).cost);

   CPPUNIT_ASSERT(result.getEdgeIdx(0, 2) != MockGraph::InvalidIndex);
   CPPUNIT_ASSERT_EQUAL(2, result.getEdge(result.getEdgeIdx(0, 2)).cost);

   CPPUNIT_ASSERT(result.getEdgeIdx(1, 3) != MockGraph::InvalidIndex);
   CPPUNIT_ASSERT_EQUAL(3, result.getEdge(result.getEdgeIdx(1, 3)).cost);

}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphDijkstra, complex)
{
   MockGraph graph;
   graph.addNode(MockNode());
   graph.addNode(MockNode());
   graph.addNode(MockNode());
   graph.addNode(MockNode());
   graph.addNode(MockNode());
   graph.addNode(MockNode());
   graph.connect(0, MockEdge(1, 1));
   graph.connect(0, MockEdge(2, 2));
   graph.connect(2, MockEdge(1, 3));
   graph.connect(1, MockEdge(3, 4));
   graph.connect(2, MockEdge(3, 5));
   graph.connect(2, MockEdge(5, 6));
   graph.connect(3, MockEdge(4, 7));
   graph.connect(4, MockEdge(5, 8));

   MockGraph result;
   GraphDijkstra(result, graph, 3);

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, result.getEdgesCount());

   CPPUNIT_ASSERT(result.getEdgeIdx(0, 1) != MockGraph::InvalidIndex);
   CPPUNIT_ASSERT_EQUAL(7, result.getEdge(result.getEdgeIdx(0, 1)).cost);

   CPPUNIT_ASSERT(result.getEdgeIdx(1, 2) != MockGraph::InvalidIndex);
   CPPUNIT_ASSERT_EQUAL(8, result.getEdge(result.getEdgeIdx(1, 2)).cost);

}

///////////////////////////////////////////////////////////////////////////////

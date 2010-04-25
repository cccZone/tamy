#include "core-TestFramework\TestFramework.h"
#include "core\Graph.h"
#include "core\GraphBFS.h"
#include <vector>
#include <list>


///////////////////////////////////////////////////////////////////////////////

namespace   // anonymous
{
    struct SomeEdge;

    typedef Graph<int, SomeEdge> TestGraph;

    struct SomeEdge
    {
        int endIdx;
        int val;

        SomeEdge(int _endIdx, int _val) : endIdx(_endIdx), val(_val) {}

        operator TestGraph::Index() const {return endIdx;}
    };

    // ------------------------------------------------------------------------

    class TestNodeVisitor
    {
    private:
        std::vector<int>& m_nodesLog;

    public:
        TestNodeVisitor(std::vector<int>& nodesLog)
        : m_nodesLog(nodesLog) {}

        void operator()(int node)
        {
            m_nodesLog.push_back(node);
        }
    };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(Graph, addingNewElements)
{
    TestGraph graph;

    graph.addNode(3);
    graph.addNode(5);
    graph.connect(0, SomeEdge(1, 10));

    CPPUNIT_ASSERT_EQUAL((unsigned int)1, graph.getEdges(0).size());
    CPPUNIT_ASSERT_EQUAL((unsigned int)0, graph.getEdges(1).size());

    // gettting edge by idx
    SomeEdge& edge = graph.getEdge(0);
    CPPUNIT_ASSERT_EQUAL(10, edge.val);

    // getting node using the edge that connects to it
    CPPUNIT_ASSERT_EQUAL(5, graph.getNode(edge));
}

///////////////////////////////////////////////////////////////////////////////

TEST(Graph, removingEdges)
{
    TestGraph graph;

    graph.addNode(3);
    graph.addNode(5);
    graph.addNode(2);    
    graph.connect(0, SomeEdge(1, 10));
    graph.connect(1, SomeEdge(0, -10));
    graph.connect(0, SomeEdge(2, 5));

    CPPUNIT_ASSERT_EQUAL( 0, graph.getEdgeIdx(0, 1));
    CPPUNIT_ASSERT_EQUAL( 1, graph.getEdgeIdx(1, 0));
    CPPUNIT_ASSERT_EQUAL( 2, graph.getEdgeIdx(0, 2));
    CPPUNIT_ASSERT_EQUAL(-1, graph.getEdgeIdx(2, 0));
    CPPUNIT_ASSERT_EQUAL(-1, graph.getEdgeIdx(1, 2));
    CPPUNIT_ASSERT_EQUAL(-1, graph.getEdgeIdx(2, 1));

    graph.disconnect(0);

    CPPUNIT_ASSERT_EQUAL(-1, graph.getEdgeIdx(0, 1));
    CPPUNIT_ASSERT_EQUAL( 1, graph.getEdgeIdx(1, 0));
    CPPUNIT_ASSERT_EQUAL(-1, graph.getEdgeIdx(0, 2));
    CPPUNIT_ASSERT_EQUAL(-1, graph.getEdgeIdx(2, 0));
    CPPUNIT_ASSERT_EQUAL(-1, graph.getEdgeIdx(1, 2));
    CPPUNIT_ASSERT_EQUAL(-1, graph.getEdgeIdx(2, 1));

}

///////////////////////////////////////////////////////////////////////////////

TEST(Graph, GraphBFS)
{
    TestGraph graph;

    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(3);
    graph.addNode(4);
    graph.addNode(5); // unconnected node
    graph.connect(0, SomeEdge(1, 0));
    graph.connect(0, SomeEdge(2, 0));
    graph.connect(1, SomeEdge(3, 0));

    std::vector<int> visitedNodes;
    TestNodeVisitor visitor(visitedNodes);
    GraphBFS(graph, 0, visitor);

    CPPUNIT_ASSERT_EQUAL((unsigned int)4, visitedNodes.size());
    CPPUNIT_ASSERT_EQUAL(1, visitedNodes[0]);
    CPPUNIT_ASSERT_EQUAL(2, visitedNodes[1]);
    CPPUNIT_ASSERT_EQUAL(3, visitedNodes[2]);
    CPPUNIT_ASSERT_EQUAL(4, visitedNodes[3]);
}

///////////////////////////////////////////////////////////////////////////////

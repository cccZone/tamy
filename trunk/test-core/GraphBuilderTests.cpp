#include "core-TestFramework\TestFramework.h"
#include "core\Graph.h"
#include "core\GraphBuilder.h"
#include "core\GraphBFS.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   struct MockNode
   {
      std::string id;

      MockNode() : id("") {}
      MockNode(const std::string& _id) : id(_id) {}
   };

   // -------------------------------------------------------------------------

   typedef Graph<MockNode, DefaultEdge<MockNode> > MockGraph;
   typedef GraphBuilder<MockNode, DefaultEdge<MockNode> > MockGraphBuilder;

   // -------------------------------------------------------------------------

   class GraphBuilderFactoryMock : public GraphBuilderFactory<MockNode, DefaultEdge<MockNode> >
   {
   public:
      MockNode createStartNode() 
      {
         return MockNode();
      }

      MockNode createNode(const std::string& name) 
      {
         return MockNode(name);
      }

      DefaultEdge<MockNode> createEdge(MockGraph::Index endNodeIdx) 
      {
         return DefaultEdge<MockNode>(endNodeIdx);
      }
   };

   // -------------------------------------------------------------------------
   
   class GraphOperationMock
   {
   private:
      std::vector<std::string>& m_callSeq;
   public:
      GraphOperationMock(std::vector<std::string>& callSeq)
         : m_callSeq(callSeq)
      {}

      void operator()(const MockNode& node)
      {
         if (node.id.length() > 0)
         {
            m_callSeq.push_back(node.id);
         }
      }
   };

}  // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(GraphBuilder, twoDependentMechanisms)
{
   std::vector<std::string> callSeq;

   MockGraph graph;
   GraphBuilderFactoryMock factory;
   MockGraphBuilder builder(factory, graph);

   PipelineChunk chunk1("fullScene");
   PipelineChunk chunk2("postProcess");
   chunk2.definePrerequisite("fullScene");
   builder.addChunk(chunk1);
   builder.addChunk(chunk2);

   GraphOperationMock op(callSeq);
   GraphBFS(graph, 0, op);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, callSeq.size());
   CPPUNIT_ASSERT_EQUAL(std::string("fullScene"), callSeq[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("postProcess"), callSeq[1]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphBuilder, twoParallelMechanisms)
{
   std::vector<std::string> callSeq;

   MockGraph graph;
   GraphBuilderFactoryMock factory;
   MockGraphBuilder builder(factory, graph);

   PipelineChunk chunk1("staticScene");
   PipelineChunk chunk2("dynamicScene");
   builder.addChunk(chunk1);
   builder.addChunk(chunk2);

   GraphOperationMock op(callSeq);
   GraphBFS(graph, 0, op);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, callSeq.size());
   CPPUNIT_ASSERT_EQUAL(std::string("staticScene"), callSeq[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("dynamicScene"), callSeq[1]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphBuilder, complexPipeline)
{
   std::vector<std::string> callSeq;

   MockGraph graph;
   GraphBuilderFactoryMock factory;
   MockGraphBuilder builder(factory, graph);

   PipelineChunk chunk1("staticOpaqueScene");
   PipelineChunk chunk2("dynamicScene");
   PipelineChunk chunk3("staticTransparentScene");
   chunk3.definePrerequisite("staticOpaqueScene");
   chunk3.definePrerequisite("dynamicScene");
   PipelineChunk chunk4("postProcess");
   chunk4.definePrerequisite("staticOpaqueScene");
   chunk4.definePrerequisite("dynamicScene");
   chunk4.definePrerequisite("staticTransparentScene");
   builder.addChunk(chunk1);
   builder.addChunk(chunk2);
   builder.addChunk(chunk3);
   builder.addChunk(chunk4);

   GraphOperationMock op(callSeq);
   GraphBFS(graph, 0, op);
   CPPUNIT_ASSERT_EQUAL((unsigned int)8, callSeq.size());
   CPPUNIT_ASSERT_EQUAL(std::string("staticOpaqueScene"), callSeq[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("dynamicScene"), callSeq[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("staticTransparentScene"), callSeq[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("postProcess"), callSeq[3]);
   CPPUNIT_ASSERT_EQUAL(std::string("staticTransparentScene"), callSeq[4]);
   CPPUNIT_ASSERT_EQUAL(std::string("postProcess"), callSeq[5]);
   CPPUNIT_ASSERT_EQUAL(std::string("postProcess"), callSeq[6]);
   CPPUNIT_ASSERT_EQUAL(std::string("postProcess"), callSeq[7]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphBuilder, reversedDefinitionOrder)
{
   std::vector<std::string> callSeq;

   MockGraph graph;
   GraphBuilderFactoryMock factory;
   MockGraphBuilder builder(factory, graph);

   PipelineChunk chunk1("staticOpaqueScene");
   PipelineChunk chunk2("dynamicScene");
   PipelineChunk chunk3("staticTransparentScene");
   chunk3.definePrerequisite("staticOpaqueScene");
   chunk3.definePrerequisite("dynamicScene");
   PipelineChunk chunk4("postProcess");
   chunk4.definePrerequisite("staticOpaqueScene");
   chunk4.definePrerequisite("dynamicScene");
   chunk4.definePrerequisite("staticTransparentScene");

   builder.addChunk(chunk4);
   builder.addChunk(chunk3);
   builder.addChunk(chunk2);
   builder.addChunk(chunk1);

   GraphOperationMock op(callSeq);
   GraphBFS(graph, 0, op);
   CPPUNIT_ASSERT_EQUAL((unsigned int)8, callSeq.size());
   CPPUNIT_ASSERT_EQUAL(std::string("dynamicScene"), callSeq[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("staticOpaqueScene"), callSeq[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("postProcess"), callSeq[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("staticTransparentScene"), callSeq[3]);
   CPPUNIT_ASSERT_EQUAL(std::string("postProcess"), callSeq[4]);
   CPPUNIT_ASSERT_EQUAL(std::string("staticTransparentScene"), callSeq[5]);
   CPPUNIT_ASSERT_EQUAL(std::string("postProcess"), callSeq[6]);
   CPPUNIT_ASSERT_EQUAL(std::string("postProcess"), callSeq[7]);
}

///////////////////////////////////////////////////////////////////////////////

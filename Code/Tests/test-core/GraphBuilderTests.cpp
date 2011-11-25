#include "core-TestFramework\TestFramework.h"
#include "core\Graph.h"
#include "core\GraphBuilder.h"
#include "core\GraphAlgorithms.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class GBMockNode;
   class MockOutput : public GBNodeOutput< GBMockNode >
   {
   public:
      MockOutput() : GBNodeOutput< GBMockNode >( "Output" ) {}
   };

   // -------------------------------------------------------------------------

   class MockInput : public GBNodeInput< MockOutput >
   {
   public:
      MockInput() : GBNodeInput< MockOutput >( "Input" ) {}

   protected:
      bool canConnect( MockOutput& output ) const { return true; }
   };

   // -------------------------------------------------------------------------

   class GBMockNode : public GraphBuilderNode< GBMockNode, MockInput, MockOutput >
   {
   public:
      int         m_idx;

   public:
      GBMockNode( int idx ) : m_idx( idx ) {}
      virtual ~GBMockNode() {}
   };

   // -------------------------------------------------------------------------

   class GBMStartNode : public GBMockNode
   {
   public:
      GBMStartNode( int idx ) : GBMockNode( idx ) 
      {
         defineOutput( new MockOutput() );
      }
   };

   // -------------------------------------------------------------------------

   class GBMEndNode : public GBMockNode
   {
   public:
      GBMEndNode( int idx ) : GBMockNode( idx ) 
      {
         defineInput( new MockInput() );
      }
   };

   // -------------------------------------------------------------------------

   class GBMUtilNode : public GBMockNode
   {
   public:
      int         m_idx;

   public:
      GBMUtilNode( int idx ) : GBMockNode( idx )
      {
         defineInput( new MockInput() );
         defineOutput( new MockOutput() );
      }
   };

   // -------------------------------------------------------------------------

   class GraphBuilderMock : public GraphBuilder< GraphBuilderMock, GBMockNode >
   {
   public:
      // -----------------------------------------------------------------
      // GraphBuilder implementation
      // -----------------------------------------------------------------
      void onNodeAdded( GBMockNode* node ) {}
      void onNodeRemoved( GBMockNode& node ) {}
   };
}

///////////////////////////////////////////////////////////////////////////////

TEST( GraphBuilderInstantiation, simpleGraph )
{
   GraphBuilderMock builder;

   GBMockNode* node[] = { new GBMStartNode( 0 ), new GBMUtilNode( 1 ), new GBMEndNode( 2 ) };

   builder.addNode( node[0] );
   builder.addNode( node[1] );
   builder.addNode( node[2] );

   node[0]->connect( "Output", *node[1], "Input" );
   node[1]->connect( "Output", *node[2], "Input" );

   Graph< GBMockNode* > graph;
   builder.buildGraph< GBMUtilNode >( graph );

   std::vector< Graph< GBMockNode* >::Index > result;
   GraphTopologicalSort( result, graph );

   CPPUNIT_ASSERT_EQUAL( (unsigned int)3, result.size() );
   CPPUNIT_ASSERT_EQUAL( node[0], graph.getNode( result[0] ) );
   CPPUNIT_ASSERT_EQUAL( node[1], graph.getNode( result[1] ) );
   CPPUNIT_ASSERT_EQUAL( node[2], graph.getNode( result[2] ) );
}

///////////////////////////////////////////////////////////////////////////////

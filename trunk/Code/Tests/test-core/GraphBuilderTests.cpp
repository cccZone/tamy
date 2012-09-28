#include "core-TestFramework\TestFramework.h"
#include "core\Graph.h"
#include "core\GraphBuilder.h"
#include "core\GraphBuilderTransaction.h"
#include "core\GraphAlgorithms.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class GBMockNode;
   class MockOutput : public GBNodeOutput< GBMockNode >
   {
      DECLARE_CLASS()

   public:
      MockOutput() : GBNodeOutput< GBMockNode >( "Output" ) {}
   };
   BEGIN_OBJECT( MockOutput );
   END_OBJECT();

   // -------------------------------------------------------------------------

   class MockInput : public GBNodeInput< GBMockNode >
   {
      DECLARE_CLASS()

   public:
      MockInput() : GBNodeInput< GBMockNode >( "Input" ) {}
   };
   BEGIN_OBJECT( MockInput );
   END_OBJECT();

   // -------------------------------------------------------------------------

   class GBMockNode : public GraphBuilderNode< GBMockNode >
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
         std::vector< GBNodeOutput< GBMockNode >* > outputs;
         outputs.push_back( new MockOutput() );

         defineOutputs( outputs );
      }
   };

   // -------------------------------------------------------------------------

   class GBMEndNode : public GBMockNode
   {
   public:
      GBMEndNode( int idx ) : GBMockNode( idx ) 
      {
         std::vector< GBNodeInput< GBMockNode >* > inputs;
         inputs.push_back( new MockInput() );

         defineInputs( inputs );
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
         std::vector< GBNodeInput< GBMockNode >* > inputs;
         inputs.push_back( new MockInput() );
         defineInputs( inputs );

         std::vector< GBNodeOutput< GBMockNode >* > outputs;
         outputs.push_back( new MockOutput() );
         defineOutputs( outputs );
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

DEFINE_TYPE_ID( MockOutput );
DEFINE_TYPE_ID( MockInput );

///////////////////////////////////////////////////////////////////////////////

TEST( GraphBuilderInstantiation, simpleGraph )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< MockOutput >( "MockOutput", new TSerializableTypeInstantiator< MockOutput >() );
   typesRegistry.addSerializableType< MockInput >( "MockInput", new TSerializableTypeInstantiator< MockInput >() );

   GraphBuilderMock builder;

   GBMockNode* node[] = { new GBMStartNode( 0 ), new GBMUtilNode( 1 ), new GBMEndNode( 2 ) };
   GraphBuilderTransaction< GraphBuilderMock, GBMockNode > transaction( builder );
   transaction.addNode( node[0] );
   transaction.addNode( node[1] );
   transaction.addNode( node[2] );
   transaction.commit();
   transaction.connectNodes( node[0], "Output", node[1], "Input" );
   transaction.connectNodes( node[1], "Output", node[2], "Input" );

   Graph< GBMockNode* > graph;
   builder.buildGraph< GBMUtilNode >( graph );

   std::vector< Graph< GBMockNode* >::Index > result;
   GraphTopologicalSort( result, graph );

   CPPUNIT_ASSERT_EQUAL( (unsigned int)3, result.size() );
   CPPUNIT_ASSERT_EQUAL( node[0], graph.getNode( result[0] ) );
   CPPUNIT_ASSERT_EQUAL( node[1], graph.getNode( result[1] ) );
   CPPUNIT_ASSERT_EQUAL( node[2], graph.getNode( result[2] ) );

   // clear the types registry
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

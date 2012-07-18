#ifndef _PIPELINE_LAYOUT_H
#error "This file can only be included from PipelineLayout.h"
#else

#include "PipelineBlock.h"


///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
PipelineLayout< TPipeline, TNode >::PipelineLayout( const FilePath& resourceName )
   : Resource( resourceName )
   , m_pipeline( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
PipelineLayout< TPipeline, TNode >::~PipelineLayout()
{
}


///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void PipelineLayout< TPipeline, TNode >::onResourceLoaded( ResourcesManager& mgr ) 
{
   if ( m_pipeline == NULL )
   {
      // the model was not set - create an instance
      std::string resourcePath = getFilePath();
      m_pipeline = mgr.findResource< TPipeline >( resourcePath );
      if ( m_pipeline == NULL )
      {

         m_pipeline = new TPipeline( resourcePath );
         mgr.addResource( m_pipeline );
      }
   }

   // remove non-existing blocks
   int count = (int)m_blocks.size();
   for ( int i = count - 1; i >= 0; --i )
   {
      if ( m_blocks[i] == NULL )
      {
         m_blocks.erase( m_blocks.begin() + i );
      }
   }

   // go through the existing blocks and set up their the sockets factory
   count = (int)m_blocks.size();
   for ( int i = 0; i < count; ++i )
   {
      initSocketsFactory( *( static_cast< PipelineBlock* >( m_blocks[i] ) ) );
   }

   restoreState();
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
GraphBlock* PipelineLayout< TPipeline, TNode >::createNode( const SerializableReflectionType& type )
{
   TNode* node = type.instantiate< TNode >();
   PipelineBlock* block = create( *node );
   initSocketsFactory( *block );
   m_pipeline->addNode( node );
   block->initialize();

   return block;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void PipelineLayout< TPipeline, TNode >::removeNode( ReflectionObject& node )
{
   TNode* rpNode = dynamic_cast< TNode* >( &node );
   ASSERT_MSG( rpNode != NULL, "Attempting to remove a node that's not a GraphBuilderNode" );

   m_pipeline->removeNode( *rpNode );
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void PipelineLayout< TPipeline, TNode >::getNodesClasses( std::vector< const SerializableReflectionType* >& classes )
{
   ReflectionTypesRegistry& classesReg = ReflectionTypesRegistry::getInstance();
   classesReg.getMatchingSerializableTypes< TNode >( classes, false );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PIPELINE_LAYOUT_H

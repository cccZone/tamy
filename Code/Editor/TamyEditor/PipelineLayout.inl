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

   // go through the existing blocks and set up their the sockets factory
   for ( std::vector< GraphBlock* >::iterator it = m_blocks.begin(); it != m_blocks.end(); ++it )
   {
      initSocketsFactory( *dynamic_cast< PipelineBlock* >( *it ) );
   }

   restoreState();
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
GraphBlock* PipelineLayout< TPipeline, TNode >::createNode( const Class& type )
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
void PipelineLayout< TPipeline, TNode >::removeNode( Object& node )
{
   TNode* rpNode = dynamic_cast< TNode* >( &node );
   ASSERT_MSG( rpNode != NULL, "Attempting to remove a node that's not a GraphBuilderNode" );

   m_pipeline->removeNode( *rpNode );
}

///////////////////////////////////////////////////////////////////////////////

template< typename TPipeline, typename TNode >
void PipelineLayout< TPipeline, TNode >::getNodesClasses( std::vector< Class >& classes )
{
   ClassesRegistry& classesReg = getClassesRegistry();
   classesReg.getClassesMatchingType< TNode >( classes );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PIPELINE_LAYOUT_H

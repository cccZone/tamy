#ifndef _RENDERING_PIPELINE_BLOCK_H
#error "This file can only be included from RenderingPipelineBlock.h"
#else

///////////////////////////////////////////////////////////////////////////////

template< typename T >
TRenderingPipelineBlock< T >::TRenderingPipelineBlock( T& node )
   : RenderingPipelineBlock( node )
   , m_nodePtr( &node )
{
   m_nodePtr->attachObserver( *this );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
TRenderingPipelineBlock< T >::~TRenderingPipelineBlock()
{
   T& nodePtr = dynamic_cast< T& >( getNode() );
   nodePtr.detachObserver( *this );

   delete m_node;
   m_node = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TRenderingPipelineBlock< T >::initialize( RenderingPipeline& parentResource )
{
   m_node = new TResourceHandle< T >( *m_nodePtr );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
Object& TRenderingPipelineBlock< T >::getNode() 
{ 
   return m_node->get();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TRenderingPipelineBlock< T >::onObjectLoaded()
{
   __super::onObjectLoaded();

   T& nodePtr = dynamic_cast< T& >( getNode() );
   nodePtr.attachObserver( *this );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TRenderingPipelineBlock< T >::update( RenderingPipelineNode& node )
{
   refreshInputs( node );
   refreshOutputs( node );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TRenderingPipelineBlock< T >::update( RenderingPipelineNode& node, const RenderingPipelineNodeOperation& msg )
{
   if ( msg == RPNO_INPUTS_CHANGED )
   {
      refreshInputs( node );
   }
   else if ( msg == RPNO_OUTPUTS_CHANGED )
   {
      refreshOutputs( node );
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _RENDERING_PIPELINE_BLOCK_H

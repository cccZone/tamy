#include "core-Renderer/RenderingPipelineNode.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( RenderingPipelineNode, ResourceObject );
   PROPERTY( InputsMap, m_inputs );
   PROPERTY( OutputsMap, m_outputs );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineNode::~RenderingPipelineNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   onBulkSocketsInitialization();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::onPropertyChanged( Property& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_inputs" )
   {
      notify( GBNO_INPUTS_CHANGED );
   }
   else if ( property.getName() == "m_outputs" )
   {
      notify( GBNO_OUTPUTS_CHANGED );
   }
   else
   {
      notify( GBNO_CHANGED );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::createLayout( RenderingPipelineMechanism& host ) const
{
   for( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->createLayout( host );
   }

   onCreateLayout( host );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::initialize( RenderingPipelineMechanism& host ) const
{
   for( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->initialize( host );
   }

   onInitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::deinitialize( RenderingPipelineMechanism& host ) const
{
   onDeinitialize( host );

   for( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->deinitialize( host );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::update( RenderingPipelineMechanism& host ) const
{
   onUpdate( host );
}

///////////////////////////////////////////////////////////////////////////////

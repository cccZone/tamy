#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RenderingPipelineNode );
   PARENT( ResourceObject );
   PROPERTY( InputsMap, m_inputs );
   PROPERTY( OutputsMap, m_outputs );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineNode::~RenderingPipelineNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::onPropertyChanged( ReflectionProperty& property )
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
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::createLayout( RenderingPipelineMechanism& host ) const
{
   for( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->createLayout( host.data() );
   }

   onCreateLayout( host );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::destroyLayout( RenderingPipelineMechanism& host ) const
{
   onDestroyLayout( host );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::update( RenderingPipelineMechanism& host ) const
{
   onUpdate( host );
}

///////////////////////////////////////////////////////////////////////////////

GBNodeInput< RenderingPipelineNode >* RenderingPipelineNode::createInput( const ReflectionType& dataType, const std::string& name ) const
{
   if ( dataType.m_name == "bool" )
   {
      return new RPBoolInput( name );
   } 
   else if ( dataType.m_name == "float" )
   {
      return new RPFloatInput( name );
   } 
   else if ( dataType.m_name == "int" )
   {
      return new RPIntInput( name );
   } 
   else if ( dataType.m_name == "Matrix" )
   {
      return new RPMatrixInput( name );
   } 
   else if ( dataType.m_name == "std::string" )
   {
      return new RPStringInput( name );
   } 
   else if ( dataType.m_name == "ShaderTexture" )
   {
      return new RPTextureInput( name );
   } 
   else if ( dataType.m_name == "Vector" )
   {
      return new RPVec4Input( name );
   } 

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

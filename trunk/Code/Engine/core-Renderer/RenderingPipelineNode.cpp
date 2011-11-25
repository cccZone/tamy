#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
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
      (*it)->createLayout( host.data() );
   }

   onCreateLayout( host );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::update( RenderingPipelineMechanism& host ) const
{
   onUpdate( host );
}

///////////////////////////////////////////////////////////////////////////////

GBNodeInput< RenderingPipelineNode >* RenderingPipelineNode::createInput( const Class& dataType, const std::string& name ) const
{
   const std::string& dataTypeName = dataType.getShortName(); 

   if ( dataTypeName == "bool" )
   {
      return new RPBoolInput( name );
   } 
   else if ( dataTypeName == "float" )
   {
      return new RPFloatInput( name );
   } 
   else if ( dataTypeName == "int" )
   {
      return new RPIntInput( name );
   } 
   else if ( dataTypeName == "D3DXMATRIX" )
   {
      return new RPMatrixInput( name );
   } 
   else if ( dataTypeName == "std::string" )
   {
      // TODO : check this type name
      return new RPStringInput( name );
   } 
   else if ( dataTypeName == "ShaderTexture" )
   {
      return new RPTextureInput( name );
   } 
   else if ( dataTypeName == "D3DXVECTOR4" )
   {
      return new RPVec4Input( name );
   } 

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

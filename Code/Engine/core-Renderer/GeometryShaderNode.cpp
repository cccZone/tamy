#include "core-Renderer/GeometryShaderNode.h"
#include "core-Renderer/GeometryEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( GeometryShaderNode, ResourceObject );
   PROPERTY( InputsMap, m_inputs );
   PROPERTY( OutputsMap, m_outputs );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GeometryShaderNode::~GeometryShaderNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   onBulkSocketsInitialization();
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderNode::onPropertyChanged( Property& property )
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

void GeometryShaderNode::createLayout( const GeometryEntity& host ) const
{
   for( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->createLayout( host.data() );
   }

   onCreateLayout( host );
}

///////////////////////////////////////////////////////////////////////////////

GBNodeInput< GeometryShaderNode >* GeometryShaderNode::createInput( const Class& dataType, const std::string& name ) const
{
   const std::string& dataTypeName = dataType.getShortName(); 

   if ( dataTypeName == "float" )
   {
      return new GSFloatInput( name );
   } 
   else if ( dataTypeName == "D3DXMATRIX" )
   {
      return new GSMatrixInput( name );
   } 
   else if ( dataTypeName == "ShaderTexture" )
   {
      return new GSTextureInput( name );
   } 
   else if ( dataTypeName == "D3DXVECTOR4" )
   {
      return new GSVec4Input( name );
   }
   else if ( dataTypeName == "bool" )
   {
      return new GSBoolInput( name );
   } 

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

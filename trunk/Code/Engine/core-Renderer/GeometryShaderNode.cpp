#include "core-Renderer/GeometryShaderNode.h"
#include "core-Renderer/GeometryEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GeometryShaderNode );
   PARENT( ResourceObject );
   PROPERTY( InputsMap, m_inputs );
   PROPERTY( OutputsMap, m_outputs );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GeometryShaderNode::~GeometryShaderNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderNode::onPropertyChanged( ReflectionProperty& property )
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

void GeometryShaderNode::createLayout( const GeometryEntity& host ) const
{
   for( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->createLayout( host.data() );
   }

   onCreateLayout( host );
}

///////////////////////////////////////////////////////////////////////////////

GBNodeInput< GeometryShaderNode >* GeometryShaderNode::createInput( const ReflectionType& dataType, const std::string& name ) const
{
   if ( dataType.m_name == "float" )
   {
      return new GSFloatInput( name );
   } 
   else if ( dataType.m_name == "Matrix" )
   {
      return new GSMatrixInput( name );
   } 
   else if ( dataType.m_name == "ShaderTexture" )
   {
      return new GSTextureInput( name );
   } 
   else if ( dataType.m_name == "Vector" )
   {
      return new GSVec4Input( name );
   }
   else if ( dataType.m_name == "bool" )
   {
      return new GSBoolInput( name );
   } 

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

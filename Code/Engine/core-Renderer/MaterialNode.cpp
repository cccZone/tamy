#include "core-Renderer/MaterialNode.h"
#include "core-Renderer/MaterialEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MaterialNode );
   PARENT( ResourceObject );
   PROPERTY( InputsMap, m_inputs );
   PROPERTY( OutputsMap, m_outputs );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MaterialNode::~MaterialNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void MaterialNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   onBulkSocketsInitialization();
}

///////////////////////////////////////////////////////////////////////////////

void MaterialNode::onPropertyChanged( ReflectionProperty& property )
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

void MaterialNode::createLayout( const MaterialEntity& host ) const
{
   for( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->createLayout( host.data() );
   }

   onCreateLayout( host );
}

///////////////////////////////////////////////////////////////////////////////

GBNodeInput< MaterialNode >* MaterialNode::createInput( const ReflectionType& dataType, const std::string& name ) const
{
   if ( dataType.m_name == "float" )
   {
      return new MSFloatInput( name );
   } 
   else if ( dataType.m_name == "Matrix" )
   {
      return new MSMatrixInput( name );
   } 
   else if ( dataType.m_name == "ShaderTexture" )
   {
      return new MSTextureInput( name );
   } 
   else if ( dataType.m_name == "Vector" )
   {
      return new MSVec4Input( name );
   }
   else if ( dataType.m_name == "bool" )
   {
      return new MSBoolInput( name );
   } 

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

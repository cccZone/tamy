#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( MaterialNode, ResourceObject );
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

void MaterialNode::onPropertyChanged( Property& property )
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

GBNodeInput< MaterialNode >* MaterialNode::createInput( const Class& dataType, const std::string& name ) const
{
   const std::string& dataTypeName = dataType.getShortName(); 

   if ( dataTypeName == "float" )
   {
      return new MSFloatInput( name );
   } 
   else if ( dataTypeName == "D3DXMATRIX" )
   {
      return new MSMatrixInput( name );
   } 
   else if ( dataTypeName == "ShaderTexture" )
   {
      return new MSTextureInput( name );
   } 
   else if ( dataTypeName == "D3DXVECTOR4" )
   {
      return new MSVec4Input( name );
   } 

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

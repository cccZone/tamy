#include "core-Renderer/GNVec4.h"
#include "core-Renderer/GeometrySockets.h"
#include "core-Renderer/GeometryEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GNVec4 );
   PARENT( GeometryShaderNode );
   PROPERTY_EDIT( "vector", Vector, m_vector );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GNVec4::GNVec4()
   : m_output( NULL )
{
   bool isBeingDeserialized = SerializationFlag::getInstance().isSerializationInProgress();
   if ( !isBeingDeserialized )
   {
      m_output = new GSVec4Output( "Vec" );
      defineOutput( m_output );
   }
}

///////////////////////////////////////////////////////////////////////////////

GNVec4::~GNVec4()
{
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GNVec4::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< GSVec4Output >( findOutput( "Vec" ) );
}

///////////////////////////////////////////////////////////////////////////////

void GNVec4::preRender( Renderer& renderer, const GeometryEntity& entity ) const
{
   m_output->setValue( entity.data(), m_vector );
}

///////////////////////////////////////////////////////////////////////////////

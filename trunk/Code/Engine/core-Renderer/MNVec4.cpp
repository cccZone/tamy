#include "core-Renderer/MNVec4.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialInstance.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNVec4 );
   PARENT( MaterialNode );
   PROPERTY_EDIT( "vector", Vector, m_vector );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MNVec4::MNVec4()
   : m_output( NULL )
{
   bool isBeingDeserialized = SerializationFlag::getInstance().isSerializationInProgress();
   if ( !isBeingDeserialized )
   {
      m_output = new MSVec4Output( "Vec" );
      defineOutput( m_output );
   }
}

///////////////////////////////////////////////////////////////////////////////

MNVec4::~MNVec4()
{
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNVec4::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< MSVec4Output >( findOutput( "Vec" ) );
}

///////////////////////////////////////////////////////////////////////////////

void MNVec4::preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const
{
   m_output->setValue( instance.data(), m_vector );
}

///////////////////////////////////////////////////////////////////////////////

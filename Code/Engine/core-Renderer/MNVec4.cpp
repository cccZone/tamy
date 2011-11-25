#include "core-Renderer/MNVec4.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNVec4, MaterialNode )
   PROPERTY_EDIT( "x", float, m_x )
   PROPERTY_EDIT( "y", float, m_y )
   PROPERTY_EDIT( "z", float, m_z )
   PROPERTY_EDIT( "w", float, m_w )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

MNVec4::MNVec4()
   : m_x( 0.0f )
   , m_y( 0.0f )
   , m_z( 0.0f )
   , m_w( 0.0f )
   , m_output( new MSVec4Output( "Vec" ) )
{
   defineOutput( m_output );
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

void MNVec4::preRender( Renderer& renderer, RuntimeDataBuffer& data ) const
{
   m_output->setValue( data, D3DXVECTOR4( m_x, m_y, m_z, m_w ) );
}

///////////////////////////////////////////////////////////////////////////////

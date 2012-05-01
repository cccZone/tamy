#include "core-Renderer/MNSurfaceProperties.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/Texture.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNSurfaceProperties );
   PARENT( MaterialNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MNSurfaceProperties::MNSurfaceProperties()
   : m_ambient( new MSVec4Output( "ambient" ) )
   , m_diffuse( new MSVec4Output( "diffuse" ) )
   , m_specular( new MSVec4Output( "specular" ) )
   , m_specularPower( new MSFloatOutput( "specularPower" ) )
   , m_emissive( new MSVec4Output( "emissive" ) )
{
   defineOutput( m_ambient );
   defineOutput( m_diffuse );
   defineOutput( m_specular );
   defineOutput( m_specularPower );
   defineOutput( m_emissive );
}

///////////////////////////////////////////////////////////////////////////////

MNSurfaceProperties::~MNSurfaceProperties()
{
   m_ambient = NULL;
   m_diffuse = NULL;
   m_specular = NULL;
   m_specularPower = NULL;
   m_emissive = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNSurfaceProperties::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_ambient = DynamicCast< MSVec4Output >( findOutput( "ambient" ) );
   m_diffuse = DynamicCast< MSVec4Output >( findOutput( "diffuse" ) );
   m_specular = DynamicCast< MSVec4Output >( findOutput( "specular" ) );
   m_specularPower = DynamicCast< MSFloatOutput >( findOutput( "specularPower" ) );
   m_emissive = DynamicCast< MSVec4Output >( findOutput( "emissive" ) );
}

///////////////////////////////////////////////////////////////////////////////

void MNSurfaceProperties::preRender( Renderer& renderer, const MaterialEntity& entity ) const
{
   const SurfaceProperties& props = entity.getSurfaceProperties();
   m_ambient->setValue( entity.data(), props.getAmbientColor() );
   m_diffuse->setValue( entity.data(), props.getDiffuseColor() );
   m_specular->setValue( entity.data(), props.getSpecularColor() );
   m_emissive->setValue( entity.data(), props.getEmissiveColor() );
   m_specularPower->setValue( entity.data(), props.getPower() );
}

///////////////////////////////////////////////////////////////////////////////

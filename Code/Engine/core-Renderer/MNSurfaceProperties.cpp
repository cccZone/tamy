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
   std::vector< GBNodeOutput< MaterialNode >* > outputs;
   outputs.push_back( m_ambient );
   outputs.push_back( m_diffuse );
   outputs.push_back( m_specular );
   outputs.push_back( m_specularPower );
   outputs.push_back( m_emissive );

   defineOutputs( outputs );
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

void MNSurfaceProperties::preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const
{
   const SurfaceProperties& props = instance.getSurfaceProperties();
   m_ambient->setValue( instance.data(), ( const Vector& )props.getAmbientColor() );
   m_diffuse->setValue( instance.data(), ( const Vector& )props.getDiffuseColor() );
   m_specular->setValue( instance.data(), ( const Vector& )props.getSpecularColor() );
   m_emissive->setValue( instance.data(), ( const Vector& )props.getEmissiveColor() );
   m_specularPower->setValue( instance.data(), props.getPower() );
}

///////////////////////////////////////////////////////////////////////////////

#include "ml-Blender/EffectCS.h"
#include "ml-Blender/BlenderScene.h"
#include "core.h"
#include "core-Renderer.h"
#include "tinyxml/tinyxml.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

EffectCS::EffectCS( TiXmlNode* effectNode, ResourcesManager& rm )
{
   if ( !effectNode )
   {
      throw std::invalid_argument( "NULL pointer instead a valid TiXmlNode effect node specified" );
   }

   TiXmlElement* effectElem = effectNode->ToElement();
   ASSERT( effectElem != NULL );

   TiXmlElement* matInstanceElem = effectElem->FirstChildElement( "instance_effect" );
   ASSERT( matInstanceElem != NULL);

   std::string url = matInstanceElem->Attribute( "url" );
   m_materialId = url.substr( url.find_first_of( '#' ) + 1 );

   // load a shader
   std::string shaderResourceName = "/Renderer/Shaders/SingleTextureEffect.tfx";
   std::string shaderFilename = "/Renderer/Shaders/SingleTextureEffect.fx";
   if ( ( m_shader = dynamic_cast< Shader* >( rm.findResource( shaderResourceName ) ) ) == NULL )
   {
      m_shader = new Shader( shaderFilename );
      rm.addResource( m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

EffectCS::~EffectCS()
{
}

///////////////////////////////////////////////////////////////////////////////

Entity* EffectCS::instantiate( const BlenderScene& host ) const
{
   SingleTextureEffect* effect = new SingleTextureEffect();
   Material& material = host.getResource< Material >( m_materialId );
   effect->setMaterial( material );

   effect->initialize( *m_shader );
   return effect;
}

///////////////////////////////////////////////////////////////////////////////

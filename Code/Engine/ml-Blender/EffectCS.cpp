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

   m_materialName = effectElem->Attribute( "name" );

   TiXmlElement* matInstanceElem = effectElem->FirstChildElement( "instance_effect" );
   ASSERT( matInstanceElem != NULL);

   std::string url = matInstanceElem->Attribute( "url" );
   m_materialId = url.substr( url.find_first_of( '#' ) + 1 );
}

///////////////////////////////////////////////////////////////////////////////

EffectCS::~EffectCS()
{
}

///////////////////////////////////////////////////////////////////////////////

Entity* EffectCS::instantiate( const BlenderScene& host ) const
{
   SingleTextureMaterial* material = new SingleTextureMaterial( m_materialName );

   // TODO: SurfaceProperties ARE NOT a resource - do something with it
  /* SurfaceProperties& surfPropertiesRes = host.getResource< SurfaceProperties >( m_materialId );
   material->setMaterial( surfPropertiesRes );*/

   return material;
}

///////////////////////////////////////////////////////////////////////////////

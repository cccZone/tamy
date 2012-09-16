#include "ml-Collada/EffectCS.h"
#include "ml-Collada/ColladaScene.h"
#include "core.h"
#include "core-Renderer.h"
#include "tinyxml/tinyxml.h"


///////////////////////////////////////////////////////////////////////////////

EffectCS::EffectCS( TiXmlNode* effectNode, const FilePath& deploymentDir, ResourcesManager& rm )
{
   if ( !effectNode )
   {
      ASSERT_MSG( false, "NULL pointer instead a valid TiXmlNode effect node specified" );
      return;
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

Entity* EffectCS::instantiate( const ColladaScene& host ) const
{
   MaterialEntity* material = BasicRenderingEntitiesFactory::createMaterial( m_materialName );

   SurfaceProperties& surfPropertiesRes = host.getResource< SurfaceProperties >( m_materialId );
   material->accessSurfaceProperties() = surfPropertiesRes;

   return material;
}

///////////////////////////////////////////////////////////////////////////////
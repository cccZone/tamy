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

   char matPath[128];
   sprintf_s( matPath, "%s%s.%s", deploymentDir.c_str(), m_materialName, MaterialInstance::getExtension() );
   m_materialInstance = rm.create< MaterialInstance >( FilePath( matPath ) );
}

///////////////////////////////////////////////////////////////////////////////

EffectCS::~EffectCS()
{
   m_materialInstance = NULL;
}

///////////////////////////////////////////////////////////////////////////////

Entity* EffectCS::instantiate( const ColladaScene& host ) const
{
   // create a new material entity
   MaterialEntity* materialEntity = BasicRenderingEntitiesFactory::createMaterial( m_materialName );

   // create a new material instance and assign it the default renderer
   {
      materialEntity->setMaterial( m_materialInstance );

      ResourcesManager& resMgr = ResourcesManager::getInstance();

      Material* materialRenderer = resMgr.create< Material >( FilePath( DEFAULT_MATERIAL_RENDERER ), true );
      m_materialInstance->setMaterialRenderer( materialRenderer );
   }

   // set the surface properties of the material instance
   SurfaceProperties& surfPropertiesRes = host.getResource< SurfaceProperties >( m_materialId );
   m_materialInstance->accessSurfaceProperties() = surfPropertiesRes;

   return materialEntity;
}

///////////////////////////////////////////////////////////////////////////////

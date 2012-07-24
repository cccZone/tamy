#include "core-Renderer/BasicRenderingEntitiesFactory.h"

#include "core-Renderer/Defines.h"

// materials
#include "core-Renderer/MaterialEntity.h"
#include "core-Renderer/Material.h"

// resources management
#include "core/ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

MaterialEntity* BasicRenderingEntitiesFactory::createMaterial( const std::string& name )
{
   MaterialEntity* entity = new MaterialEntity( name );

   ResourcesManager& resMgr = ResourcesManager::getInstance();
   std::string materialPath = std::string( MATERIALS_DIR ) + "simpleTextured.rml";
   Material* mat = resMgr.create< Material >( FilePath( materialPath ) );
   entity->setMaterial( mat );

   return entity;
}

///////////////////////////////////////////////////////////////////////////////

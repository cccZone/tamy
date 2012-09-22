#include "core-Renderer/BasicRenderingEntitiesFactory.h"

#include "core-Renderer/Defines.h"

// materials
#include "core-Renderer/MaterialEntity.h"
#include "core-Renderer/MaterialInstance.h"

// resources management
#include "core/ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

MaterialEntity* BasicRenderingEntitiesFactory::createMaterial( const std::string& name )
{
   MaterialEntity* entity = new MaterialEntity( name );

   ResourcesManager& resMgr = ResourcesManager::getInstance();
   MaterialInstance* mat = resMgr.create< MaterialInstance >( FilePath( DEFAULT_MATERIAL_INSTANCE ) );
   entity->setMaterial( mat );

   return entity;
}

///////////////////////////////////////////////////////////////////////////////

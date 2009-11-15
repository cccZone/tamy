#include "core-Scene\Entity.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   ClassesRegistry* g_entitiesRegistry = NULL;

} // anonymous

///////////////////////////////////////////////////////////////////////////////

ClassesRegistry& getEntitiesRegistry()
{
   if (g_entitiesRegistry == NULL)
   {
      g_entitiesRegistry = new ClassesRegistry();
   }
   return *g_entitiesRegistry;
}

///////////////////////////////////////////////////////////////////////////////

Entity::Entity(int classHandle)
: m_classHandle(classHandle)
{
   if (m_classHandle < 0)
   {
      throw std::invalid_argument("Invalid entity initialization. Use INIT_ENTITY macro!");
   }
}

///////////////////////////////////////////////////////////////////////////////

Entity* Entity::create(int entityClassHandle)
{
   return (Entity*)(getEntitiesRegistry().create(entityClassHandle));
}

///////////////////////////////////////////////////////////////////////////////

int Entity::getClassHandle() const
{
   return m_classHandle;
}

///////////////////////////////////////////////////////////////////////////////

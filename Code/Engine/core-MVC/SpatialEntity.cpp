#include "core-MVC\SpatialEntity.h"
#include "core\Assert.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT(SpatialEntity, Entity)
   PROPERTY_EDIT("Local transformation", D3DXMATRIX, m_localMtx)
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

SpatialEntity::SpatialEntity( const std::string& name )
: Entity( name )
, Node( name )
{
}

///////////////////////////////////////////////////////////////////////////////

SpatialEntity::~SpatialEntity() 
{
   unsigned int count = m_children.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      SpatialEntity* se = dynamic_cast<SpatialEntity*> (m_children[i]);
      removeChild(*se);
   }
}

///////////////////////////////////////////////////////////////////////////////

void SpatialEntity::onChildAttached(Entity& child) 
{
   SpatialEntity* se = dynamic_cast<SpatialEntity*> (&child);
   if (se == NULL) { return; }

   addChild(se);
}

///////////////////////////////////////////////////////////////////////////////

void SpatialEntity::onChildDetached(Entity& child)
{
   SpatialEntity* se = dynamic_cast<SpatialEntity*> (&child);
   if (se == NULL) { return; }

   removeChild(*se);
}

///////////////////////////////////////////////////////////////////////////////

void SpatialEntity::onObjectLoaded()
{
   __super::onObjectLoaded();

   // update the node name
   setName( getEntityName() );
}

///////////////////////////////////////////////////////////////////////////////

#include "SelectEntityCommand.h"
#include "core\Ray.h"
#include "core-Renderer\Camera.h"
#include "core-MVC\Entity.h"
#include "SelectionManager.h"
#include "SpatiallyQueryable.h"


///////////////////////////////////////////////////////////////////////////////

SelectEntityCommand::SelectEntityCommand( Camera& camera,
                                          QueryableScene& scene, 
                                          SelectionManager& selectionMgr )
: m_camera( camera )
, m_scene( scene )
, m_selectionMgr( selectionMgr )
{
}

///////////////////////////////////////////////////////////////////////////////

void SelectEntityCommand::execute( const D3DXVECTOR2& mousePos )
{
   Ray queryRay = m_camera.createRay(mousePos.x, mousePos.y);

   Array<SpatiallyQueryable*> objects;
   m_scene.query(queryRay, objects);

   if (objects.size() == 0) 
   {
      m_selectionMgr.resetSelection();
   }
   else
   {
      SpatiallyQueryable* closestObject = findClosest(queryRay, objects);
      if (closestObject == NULL)
      {
         throw std::logic_error("Error finding the closest object");
      }

      m_selectionMgr.selectObject(closestObject->getEntity());
   }
}

///////////////////////////////////////////////////////////////////////////////

SpatiallyQueryable* SelectEntityCommand::findClosest( const Ray& ray,
                                                      const Array<SpatiallyQueryable*>& queryables )
{
   float minDist = FLT_MAX;
   SpatiallyQueryable* closest = NULL;

   unsigned int count = queryables.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      float dist = ray.getDistanceTo(queryables[i]->getBoundingVolume());

      if (dist < minDist)
      {
         minDist = dist;
         closest = queryables[i];
      }
   }

   return closest;
}

///////////////////////////////////////////////////////////////////////////////

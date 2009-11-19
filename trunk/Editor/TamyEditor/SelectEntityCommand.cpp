#include "SelectEntityCommand.h"
#include "core\Ray.h"
#include "core-Renderer\Camera.h"
#include "core-Scene\Entity.h"


///////////////////////////////////////////////////////////////////////////////

SelectEntityCommand::SelectEntityCommand(Camera& camera,
                                         QueryableScene& scene, 
                                         const Output& selectEntity)
: m_camera(camera)
, m_scene(scene)
, m_selectEntity(selectEntity)
{
}

///////////////////////////////////////////////////////////////////////////////

void SelectEntityCommand::execute(const D3DXVECTOR2& mousePos)
{
   Ray queryRay = m_camera.createRay(mousePos.x, mousePos.y);

   Array<SpatiallyQueryable*> objects;
   m_scene.query(queryRay, objects);

   SpatiallyQueryable* closestObject = findClosest(queryRay, objects);
   Entity* closestEntity = dynamic_cast<Entity*> (closestObject);

   m_selectEntity(*closestEntity);
}

///////////////////////////////////////////////////////////////////////////////

SpatiallyQueryable* SelectEntityCommand::findClosest(const Ray& ray,
                                                     const Array<SpatiallyQueryable*>& queryables)
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

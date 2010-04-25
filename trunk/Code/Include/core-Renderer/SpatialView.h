#pragma once

/// @file   core-Renderer\SpatialView.h
/// @brief  a renderable scene view that processes the visibility 
///         of scene objects

#include "core-MVC\ModelView.h"
#include "core\RegularOctree.h"
#include <map>


///////////////////////////////////////////////////////////////////////////////

class Renderable;
class SpatialRepresentation;
class CameraContext;
struct AABoundingBox;

///////////////////////////////////////////////////////////////////////////////

/**
 * A renderable scene view that processes the visibility of scene objects.
 */
class SpatialView : public ModelView
{
private:
   typedef std::map<Renderable*, SpatialRepresentation*> RepresentationsMap;
   RepresentationsMap m_representations;

   RegularOctree<SpatialRepresentation>* m_storage;
   int m_visibilityTag;

public:
   /**
    * Constructor.
    *
    * @param sceneBB    a bounding box large enough to encompass 
    *                   the entire scene
    */
   SpatialView(const AABoundingBox& sceneBB);
   ~SpatialView();

   /**
    * Processes the objects' visibility.
    *
    * @param camera  context that will be used to determine the objects' 
    *                visibility
    */
   void update(CameraContext& camera);

   // -------------------------------------------------------------------------
   // ModelView implementation
   // -------------------------------------------------------------------------
   void onEntityAdded(Entity& entity);

   void onEntityRemoved(Entity& entity);

protected:
   void resetContents();
};

///////////////////////////////////////////////////////////////////////////////

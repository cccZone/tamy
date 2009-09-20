#pragma once

#include "core\StaticGeometryOctree.h"
#include <set>


///////////////////////////////////////////////////////////////////////////////

class StaticGeometryRenderable;

///////////////////////////////////////////////////////////////////////////////

namespace demo
{

///////////////////////////////////////////////////////////////////////////////

class StaticSceneManager;
typedef StaticGeometryOctree<StaticGeometryRenderable, 
                             StaticSceneManager> StaticSceneTree;

///////////////////////////////////////////////////////////////////////////////

/**
 * An adapter that allows to add static renderables
 * to a tree and serves as a manager for those renderables that the
 * static geometry tree requires in order to work.
 */
class StaticSceneManager : public SpatialStorage<StaticGeometryRenderable>
{
private:
   typedef std::set<StaticGeometryRenderable*> RenderablesSet;
   RenderablesSet m_renderables;

   StaticSceneTree* m_staticSceneTree;

public:
   /**
    * Constructor. 
    *
    * @param treeBB           bounding box for the tree
    * @param maxElements      when a new element is added to a sector,
    *                         the sector will check whether it doesn't exceed
    *                         this value in terms of how many elements it stores.
    *                         If it does, it will be further subdivided
    * @param maxDepth         maximum depth of the tree. Let's assume we're
    *                         adding 4 objects that have the same bounding volume
    *                         and are located on the same position, while maxElems
    *                         is set to 1. In this case, the tree would try to infinitely
    *                         subdivide itself and thus fall into an endless loop.
    *                         To prevent this from happening, this param comes
    *                         into play. It also helps stabilizing query times, by
    *                         preventing the tree to be very tall in one place, and
    *                         ultra shallow in another.
    * @param initDepth        for convenience tree can be initially subdivided to
    *                         a certain depth. This depth is limited by the value
    *                         specified in 'maxDepth' param
    */
   StaticSceneManager(const AABoundingBox& treeBB, 
                      int maxElements, 
                      int maxDepth,
                      int initDepth = 0);

   ~StaticSceneManager();

   void addObj(StaticGeometryRenderable* elem);

   // -------------------------------------------------------------------------
   // SpatialStorage implementation
   // -------------------------------------------------------------------------

   void insert(StaticGeometryRenderable& elem);

   void remove(StaticGeometryRenderable& elem);

   void query(const BoundingVolume& boundingVol, 
              Array<StaticGeometryRenderable*>& output) const;

   // -------------------------------------------------------------------------
   // Interface to a StaticGeometryOctree
   // -------------------------------------------------------------------------

   void manage(StaticGeometryRenderable* renderable);

   void unmanage(StaticGeometryRenderable& renderable);
};

///////////////////////////////////////////////////////////////////////////////

} // namespace demo

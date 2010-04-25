#pragma once

/// @file   TamyEditor\QueryableSceneProxy.h
/// @brief  a proxy to a queryable scene

#include "QueryableScene.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This proxy scene will allow to register it as a service
 * and thus assure continuity of its existence while switching the
 * underlying solid scenes this instance will proxy its calls to.
 */
class QueryableSceneProxy : public QueryableScene
{
private:
   QueryableScene* m_defaultImpl;
   QueryableScene* m_currentImpl;

public:
   QueryableSceneProxy();
   ~QueryableSceneProxy();

   // -------------------------------------------------------------------------
   // Solid implementations management
   // -------------------------------------------------------------------------
   /**
    * Sets a new solid instance to which the calls will be proxied from now on.
    * Passing NULL as a parameter will switch proxying the calls back to
    * the default implementation.
    *
    * @param scene   new solid scene instance
    */
   void plugin(QueryableScene* scene);

   // -------------------------------------------------------------------------
   // QueryableScene implementation
   // -------------------------------------------------------------------------
   void query(const BoundingVolume& boundingVol, 
              Array<SpatiallyQueryable*>& output) const;

   void insert(SpatiallyQueryable& entity);

   void remove(SpatiallyQueryable& entity);
};

///////////////////////////////////////////////////////////////////////////////

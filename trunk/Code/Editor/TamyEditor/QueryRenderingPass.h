#pragma once

/// @file   TamyEditor\QueryRenderingPass.h
/// @brief  a rendering pass dedicated to scene queries mechanism

#include "core.h"
#include "core-Renderer\RenderingPass.h"
#include <list>
#include <map>


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;
class Camera;
class RenderTarget;
class Entity;
class SceneQuery;
class QueryableEntity;
class SceneQueryEffect;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pass dedicated to scene queries mechanism.
 */
class QueryRenderingPass : public RenderingPass,
                           public GenericFactory< Entity, QueryableEntity >
{
private:
   typedef std::map< Entity*, QueryableEntity* > Representations;
   typedef std::list< SceneQuery* >              QueriesList;

private:
   RenderTarget&           m_sceneSnapshot;
   Representations         m_representations;
   SceneQueryEffect*       m_effect;
   QueriesList             m_queriesList;

public:
   /**
    * Constructor.
    *
    * @param sceneSnapshot    render target
    * @param rm               resources manager
    * @param camera           camera observing the rendered scene
    */
   QueryRenderingPass( RenderTarget& sceneSnapshot, ResourcesManager& rm, Camera& camera );
   ~QueryRenderingPass();

   /**
    * Adds a new entity for rendering.
    *
    * @param entity
    */
   void addEntity( Entity& entity );

   /**
    * Removes an entity from rendering
    */
   void removeEntity( Entity& entity );

   /**
    * Resets the pass, removing all added entities.
    */
   void reset();

   /**
    * Registers a new scene query.
    *
    * @param query
    */
   void query( SceneQuery& query );

protected:
   // -------------------------------------------------------------------------
   // RenderingPass implementation
   // -------------------------------------------------------------------------
   bool onPreRender();
   void onPostRender();

private:
   Entity* getEntityFromPos( const D3DXVECTOR2& pos );
};

///////////////////////////////////////////////////////////////////////////////

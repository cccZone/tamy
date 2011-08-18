#pragma once

/// @file   TamyEditor\QueryRenderingPass.h
/// @brief  a rendering pass dedicated to scene queries mechanism

#include "core.h"
#include <list>
#include <map>
#include "core-Renderer/RenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

class Camera;
class RenderTarget;
class Entity;
class SceneQuery;
class PixelShader;
class QueryableEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pass dedicated to scene queries mechanism.
 */
class QueryRenderingPass : public RenderingMechanism, public GenericFactory< Entity, QueryableEntity >
{
private:
   typedef std::map< Entity*, QueryableEntity* > Representations;
   typedef std::list< SceneQuery* >              QueriesList;

private:
   RenderTarget&           m_sceneSnapshot;
   Representations         m_representations;
   PixelShader*            m_shader;
   QueriesList             m_queriesList;
   QueriesList             m_completedQueriesList;

public:
   /**
    * Constructor.
    *
    * @param sceneSnapshot    render target
    */
   QueryRenderingPass( RenderTarget& sceneSnapshot );
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
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void deinitialize( Renderer& renderer );
   void render( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

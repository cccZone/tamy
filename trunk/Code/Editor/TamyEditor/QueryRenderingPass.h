#pragma once

/// @file   TamyEditor\QueryRenderingPass.h
/// @brief  a rendering pass dedicated to scene queries mechanism

#include "core.h"
#include <list>
#include <map>
#include "core-Renderer/RenderingMechanism.h"
#include "core-MVC/ModelView.h"


///////////////////////////////////////////////////////////////////////////////

class Camera;
class RenderTarget;
class Entity;
class SceneQuery;
class PixelShader;
class QueryableEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * Queries the rendered scene for an object visible at the specified location.
 */
class QueryRenderingPass : public RenderingMechanism, public GenericFactory< Entity, QueryableEntity >, public ModelView
{
private:
   union PtrAsBytes
   {
      long ptr;
      struct
      {
         unsigned char b[4];
      };
   };

private:
   typedef std::map< Entity*, QueryableEntity* > Representations;
   typedef std::list< SceneQuery* >              QueriesList;

private:
   RenderTarget*           m_sceneSnapshot;
   Representations         m_representations;
   PixelShader*            m_shader;
   QueriesList             m_queriesList;
   QueriesList             m_completedQueriesList;
   bool                    m_debugMode;

public:
   /**
    * Constructor.
    */
   QueryRenderingPass();
   ~QueryRenderingPass();

   /**
    * Registers a new scene query.
    *
    * @param query
    */
   void query( SceneQuery& query );

   /**
    * Toggles the widget debug mode.
    */
   inline void toggleDebugMode() { m_debugMode = !m_debugMode; }

   // -------------------------------------------------------------------------
   // Helper methods
   // -------------------------------------------------------------------------
   /**
    * Converts a regular pointer to a vector, which can be used as an argument
    * of the SceneQueryEffect.
    *
    * @param ptr     pointer we want to convert
    * @return        pointer in a vector form
    */
   static D3DXVECTOR4 ptrToVec( void* ptr );

   /**
    * Inverted conversion to the one introduced by method 'ptrToVec'
    *
    * @param vec     a vector representation of a pointer
    * @return        a pointer
    */
   static void* vecToPtr( const D3DXVECTOR4& vec );

   // -------------------------------------------------------------------------
   // ModelView implementation
   // -------------------------------------------------------------------------
   void onEntityAdded( Entity& entity );
   void onEntityRemoved( Entity& entity );
   void onEntityChanged( Entity& entity );
   void resetContents();

protected:
   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void deinitialize( Renderer& renderer );
   void render( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

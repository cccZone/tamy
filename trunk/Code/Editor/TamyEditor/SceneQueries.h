#pragma once

/// @file   TamyEditor\SceneQueries.h
/// @brief  component that allows to query scene for entities

#include "core.h"
#include "core-MVC\ModelView.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class Entity;
class Model;
class SceneQuery;
class RenderTarget;
class QueryRenderingPass;

///////////////////////////////////////////////////////////////////////////////

/**
 * This component is responsible for querying a scene for the objects
 * it contains.
 */
class SceneQueries : public Component< TamyEditor >,
                     public ModelView
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
   Model*                  m_currObservedScene;

   RenderTarget*           m_sceneSnapshot;
   QueryRenderingPass*     m_renderingPass;

public:
   /**
    * Constructor.
    */
   SceneQueries();
   ~SceneQueries();

   /**
    * Registers a new scene query.
    *
    * @param query
    */
   void query( SceneQuery& query );

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
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );
   void onServiceRegistered( TamyEditor& mgr );

   // -------------------------------------------------------------------------
   // ModelView implementation
   // -------------------------------------------------------------------------
   void onEntityAdded( Entity& entity );
   void onEntityRemoved( Entity& entity );
   void onEntityChanged( Entity& entity );

protected:
   void renderRepresentations();
   void resetContents();
};

///////////////////////////////////////////////////////////////////////////////

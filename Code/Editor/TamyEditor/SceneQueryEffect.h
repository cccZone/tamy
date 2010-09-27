#pragma once

/// @file   TamyEditor/SceneQueryEffect.h
/// @brief  An effect used by the SceneQueries component

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Shader;
class Geometry;
class ResourcesManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * This effect renders pointer to the object instead of rendering its material.
 */
class SceneQueryEffect
{
private:
   Shader*           m_shader;
   D3DXVECTOR4       m_renderedPtr;

public:
   /**
    * Constructor.
    *
    * @param rm         resources manager
    * @param camera     from its perspective the objects will be rendered
    */
   SceneQueryEffect( ResourcesManager& rm );

   /**
    * The method allows to change currently rendered pointer value.
    *
    * @param newPtr     pointer to the object
    */
   inline void setObject( const D3DXVECTOR4& newPtr ) 
   { 
      m_renderedPtr = newPtr; 
   }

   /**
    * Renders the geometry
    */
   void render( Geometry& geometry );
};

///////////////////////////////////////////////////////////////////////////////

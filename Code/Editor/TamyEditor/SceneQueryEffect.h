#pragma once

/// @file   TamyEditor/SceneQueryEffect.h
/// @brief  An effect used by the SceneQueries component

#include "core-Renderer\ShaderEffect.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Camera;

///////////////////////////////////////////////////////////////////////////////

/**
 * This effect renders pointer to the object instead of rendering its material.
 */
class SceneQueryEffect : public ShaderEffect
{
private:
   Camera&           m_camera;

   D3DXMATRIX        m_objMtx;
   D3DXVECTOR4       m_renderedPtr;

public:
   /**
    * Constructor.
    *
    * @param rm         resources manager
    * @param camera     from its perspective the objects will be rendered
    */
   SceneQueryEffect( ResourcesManager& rm, Camera& camera );

   /**
    * The method allows to change currently rendered pointer value.
    *
    * @param mtx        object's transformation matrix
    * @param newPtr     pointer to the object
    */
   inline void setObject( const D3DXMATRIX& mtx, const D3DXVECTOR4& newPtr ) 
   { 
      m_objMtx = mtx; 
      m_renderedPtr = newPtr; 
   }

protected:
   // -------------------------------------------------------------------------
   // ShaderEffect implementation
   // -------------------------------------------------------------------------
   void onBeginRendering();
};

///////////////////////////////////////////////////////////////////////////////

#pragma once

/// @file   core-Renderer\PostProcessSceneStorage.h
/// @brief  storage for holding a single renderable taht can be used
///         for post process rendering

#include "core-Renderer\RenderablesStorage.h"


///////////////////////////////////////////////////////////////////////////////

class PostProcessEffectRenderable;

///////////////////////////////////////////////////////////////////////////////

/**
 * This scene storage is meant only for a single renderable
 * that can be used with post process rendering effects.
 * Its ultra simple, because post process effect renders only
 * one thing - the whole screen - but it's not about the quantity, but quality.
 */
class PostProcessSceneStorage : public RenderablesStorage
{
private:
   PostProcessEffectRenderable* m_renderable;

public:
   /**
    * Constructor.
    * 
    * @param renderable    a renderable that can be used with a post process
    *                      mechanism for rendering
    */
   PostProcessSceneStorage(PostProcessEffectRenderable* renderable);
   ~PostProcessSceneStorage();

   void query(Camera& camera, Array<Renderable*>& outRenderables);
};
///////////////////////////////////////////////////////////////////////////////

#pragma once

/// @file   core-Renderer\RenderLights.h
/// @brief  this rendering action lights the rendered scene.

#include "core-Renderer\RenderingAction.h"
#include "core\Array.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class LightsStorage;
class Light;

///////////////////////////////////////////////////////////////////////////////

/**
 * This action will render lights that shine at the point in space
 * where the rendered object is
 */
class RenderLights : public RenderingAction
{
private:
   LightsStorage& m_lightsStorage;

   Array<Light*> m_enabledLights;

public:
   /** 
    * Constructor.
    *
    * @param lightsStorage    a storage that contains the lights
    *                         we can choose from
    */
   RenderLights(LightsStorage& lightsStorage);

   void onRenderingBegin(Renderable& renderable, RendererImpl& impl);

   void onRenderingEnd(Renderable& renderable, RendererImpl& impl);
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\RenderLights.h"
#include "core-Renderer\RenderableNode.h"
#include "core-Renderer\RendererImpl.h"
#include "core-Renderer\LightsStorage.h"
#include "core-Renderer\Light.h"
#include <algorithm>
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

RenderLights::RenderLights(LightsStorage& lightsStorage)
: m_lightsStorage(lightsStorage)
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderLights::onRenderingBegin(Renderable& renderable, RendererImpl& impl)
{
   const BoundingVolume& volume = renderable.getVolume();

   m_enabledLights.clear();
   m_lightsStorage.query(volume, m_enabledLights);

   impl.enableLights(m_enabledLights);
}

///////////////////////////////////////////////////////////////////////////////

void RenderLights::onRenderingEnd(Renderable& renderable, RendererImpl& impl)
{
   impl.disableLights(m_enabledLights);
}

///////////////////////////////////////////////////////////////////////////////

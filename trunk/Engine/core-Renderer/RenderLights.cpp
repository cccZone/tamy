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
   const D3DXMATRIX& situation = renderable.getGlobalTransform();
   D3DXVECTOR3 pos(situation._41, situation._42, situation._43);

   m_enabledLights.clear();
   m_lightsStorage.query(pos, m_enabledLights);

   impl.enableLights(m_enabledLights);
}

///////////////////////////////////////////////////////////////////////////////

void RenderLights::onRenderingEnd(Renderable& renderable, RendererImpl& impl)
{
   impl.disableLights(m_enabledLights);
}

///////////////////////////////////////////////////////////////////////////////

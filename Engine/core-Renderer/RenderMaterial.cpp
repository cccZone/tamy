#include "core-Renderer\RenderMaterial.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\RendererImpl.h"
#include "core-Renderer\Material.h"


///////////////////////////////////////////////////////////////////////////////

const Material* RenderMaterial::m_prevMaterial = NULL;

///////////////////////////////////////////////////////////////////////////////

RenderMaterial::RenderMaterial()
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderMaterial::resetMaterialMemory()
{
   m_prevMaterial = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderMaterial::onRenderingBegin(Renderable& renderable, 
                                      RendererImpl& impl)
{
   const Material& material = renderable.getMaterial();

   if ((!m_prevMaterial) || (*m_prevMaterial != material))
   {
      material.setForRendering(impl);
      m_prevMaterial = &material;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderMaterial::onRenderingEnd(Renderable& renderable, 
                                    RendererImpl& impl)
{
}

///////////////////////////////////////////////////////////////////////////////

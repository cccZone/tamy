#include "core-Renderer\PostProcessEffectRenderable.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"


///////////////////////////////////////////////////////////////////////////////

PostProcessEffectRenderable::PostProcessEffectRenderable(Renderer& renderer)
      : m_renderer(renderer),
      m_defaultMat(new Material("PostProcessEffectRenderableMat"))
{
   m_renderer.attachObserver(*this);

   D3DXMatrixIdentity(&m_worldTransform);
   m_renderingMatrices.push_back(m_worldTransform);
}

///////////////////////////////////////////////////////////////////////////////

PostProcessEffectRenderable::~PostProcessEffectRenderable()
{
   
   m_renderer.detachObserver(*this);
   delete m_defaultMat;
   m_defaultMat = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessEffectRenderable::update(Renderer& renderer)
{
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessEffectRenderable::update(Renderer& renderer, const RendererOps& operation)
{
   if (operation == RO_RESIZE_VIEWPORT)
   {
      recreateGeometry(renderer.getViewportWidth(), renderer.getViewportHeight());
   }
}

///////////////////////////////////////////////////////////////////////////////

const Array<D3DXMATRIX>& PostProcessEffectRenderable::getRenderingMatrices()
{
   return m_renderingMatrices;
}

///////////////////////////////////////////////////////////////////////////////
   
const Material& PostProcessEffectRenderable::getMaterial() const
{
   return *m_defaultMat;
}

///////////////////////////////////////////////////////////////////////////////
   
void PostProcessEffectRenderable::addInputTexture(Texture& texture, 
                                                  GraphicalEntitiesFactory& factory)
{
   m_defaultMat->addStage(
         factory.createMaterialStage(texture, 
                                     MOP_SELECT_ARG1, SC_TEXTURE, SC_NONE,
                                     MOP_SELECT_ARG1, SC_TEXTURE, SC_NONE,
                                     CC_CLAMP));
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& PostProcessEffectRenderable::getGlobalTransform()
{
   return m_worldTransform;
}

///////////////////////////////////////////////////////////////////////////////

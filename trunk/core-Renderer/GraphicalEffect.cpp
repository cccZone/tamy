#include "core-Renderer\GraphicalEffect.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\EffectDataSource.h"
#include "core-Renderer\RenderingTargetsPolicy.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

GraphicalEffect::GraphicalEffect(const std::string& name, 
                                 RenderingTargetsPolicy& policy,
                                 bool isTransparent,
                                 EffectDataSource* dataSource)
      : m_name(name),
      m_renderingTargetsPolicy(policy),
      m_transparent(isTransparent),
      m_dataSource(dataSource)
{
   if (dataSource == NULL)
   {
      throw std::invalid_argument("NULL pointer instaed an EffectDataSource instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEffect::~GraphicalEffect() 
{
   delete m_dataSource;
   m_dataSource = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEffect::render(Renderable& renderable)
{
   m_dataSource->setData(*this);
   unsigned int passesCount = beginRendering();
   for (unsigned int passIdx = 0; passIdx < passesCount; ++passIdx)
   {
      m_renderingTargetsPolicy.setTargets(passIdx);

      beginPass(passIdx);
      renderable.render();

      endPass(passIdx);
   }
   endRendering();
}

///////////////////////////////////////////////////////////////////////////////

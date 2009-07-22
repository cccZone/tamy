#include "core-Renderer\GraphicalEffect.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\EffectDataSource.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

GraphicalEffect::GraphicalEffect(const std::string& name, 
                                 RenderingTargetsPolicy& policy,
                                 bool isTransparent,
                                 EffectDataSource* dataSource)
      : RenderingTechnique(name, policy),
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

unsigned int GraphicalEffect::beginRendering()
{
   m_dataSource->setData(*this);
   return onBeginRendering();
}

///////////////////////////////////////////////////////////////////////////////

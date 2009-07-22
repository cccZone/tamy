#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\TransparencyEnabler.h"
#include <string.h>
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

unsigned char Material::s_stagesArrSize = 8;

///////////////////////////////////////////////////////////////////////////////

Material::Material(const std::string& name,
                   RenderingTargetsPolicy& policy,
                   LightReflectingProperties* lrp,
                   MaterialOperationImplementation& alphaMatOp,
                   MaterialOperationImplementation& colorMatOp,
                   TransparencyEnabler& transparencyEnabler)
      : RenderingTechnique(name, policy),
      m_lightReflectingProperties(lrp),
      m_disableAlpha(new MaterialOperation(alphaMatOp, MOP_DISABLE, SC_NONE, SC_NONE)),
      m_disableColor(new MaterialOperation(colorMatOp, MOP_DISABLE, SC_NONE, SC_NONE)),
      m_transparencyEnabler(transparencyEnabler),
      m_stagesCount(0),
      m_transparent(false)
{
   m_stages = new MaterialStageP[s_stagesArrSize];
   memset(m_stages, NULL, sizeof(MaterialStageP) * s_stagesArrSize);

   checkTransparency();
}

///////////////////////////////////////////////////////////////////////////////

Material::Material(const Material& rhs)
      : RenderingTechnique(rhs),
      m_lightReflectingProperties(rhs.m_lightReflectingProperties->clone()),
      m_disableAlpha(new MaterialOperation(*rhs.m_disableAlpha)),
      m_disableColor(new MaterialOperation(*rhs.m_disableColor)),
      m_transparencyEnabler(rhs.m_transparencyEnabler),
      m_stagesCount(rhs.m_stagesCount),
      m_transparent(false)
{
   m_stages = new MaterialStageP[s_stagesArrSize];
   memset(m_stages, NULL, sizeof(MaterialStageP) * s_stagesArrSize);

   // copy material stages
   for (int i = 0; i < m_stagesCount; ++i)
   {
      m_stages[i] = new MaterialStage(*rhs.m_stages[i]);
   }

   checkTransparency();
}

///////////////////////////////////////////////////////////////////////////////

Material::~Material()
{
   delete m_lightReflectingProperties;
   m_lightReflectingProperties = NULL;

   delete m_disableAlpha;
   m_disableAlpha = NULL;

   delete m_disableColor;
   m_disableColor = NULL;

   for (unsigned char i = 0; i < m_stagesCount; ++i)
   {
      delete m_stages[i];
   }
   delete [] m_stages;
   m_stages = NULL;

   m_stagesCount = 0;
}

///////////////////////////////////////////////////////////////////////////////

void Material::setLightReflectingProperties(LightReflectingProperties* lrp)
{
   if (lrp == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a LightReflectingProperties instance");
   }

   delete m_lightReflectingProperties;
   m_lightReflectingProperties = lrp;
}

///////////////////////////////////////////////////////////////////////////////

void Material::addStage(MaterialStage* stage)
{
   if (stage == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a MaterialStage instance");
   }

   if (m_stagesCount >= s_stagesArrSize)
   {
      throw std::out_of_range("Too many stages defined");
   }

   m_stages[m_stagesCount] = stage;
   stage->setIndex(m_stagesCount);
   m_stagesCount++;

   checkTransparency();
}

///////////////////////////////////////////////////////////////////////////////

void Material::removeStage(unsigned int stageIdx)
{
   if (stageIdx >= m_stagesCount)
   {
      throw std::out_of_range("Invalid stage index - there aren't so many defined in this material");
   }

   delete m_stages[stageIdx];
   unsigned char newIdx;
   for (unsigned char i = stageIdx + 1; i < m_stagesCount; ++i)
   {
      newIdx = i - 1;
      m_stages[newIdx] = m_stages[i];
      m_stages[newIdx]->setIndex(newIdx);
   }
   m_stagesCount--;

   checkTransparency();
}

///////////////////////////////////////////////////////////////////////////////

MaterialStage& Material::getStage(unsigned int stageIdx)
{
   if (stageIdx >= m_stagesCount)
   {
      throw std::out_of_range("Invalid stage index - there aren't so many defined in this material");
   }

   return *(m_stages[stageIdx]);
}

///////////////////////////////////////////////////////////////////////////////

bool Material::isTransparent() const
{
   return m_transparent;
}

///////////////////////////////////////////////////////////////////////////////

unsigned int Material::beginRendering()
{
   m_transparencyEnabler.setTransparency(m_transparent);

   m_lightReflectingProperties->setForRendering();

   for (unsigned char i = 0; i < m_stagesCount; ++i)
   {
      m_stages[i]->setForRendering();
   }

   m_disableAlpha->setForRendering(m_stagesCount);
   m_disableColor->setForRendering(m_stagesCount);

   return 1;
}

///////////////////////////////////////////////////////////////////////////////

void Material::endRendering()
{
}

///////////////////////////////////////////////////////////////////////////////

void Material::beginPass(const unsigned int& passIdx)
{
}

///////////////////////////////////////////////////////////////////////////////

void Material::endPass(const unsigned int& passIdx)
{
}

///////////////////////////////////////////////////////////////////////////////

void Material::checkTransparency()
{
   m_transparent = false;
   for (unsigned char i = 0; i < m_stagesCount; ++i)
   {
      if (m_stages[i]->getAlphaOperation().getOperationCode() != MOP_DISABLE) 
      {
         m_transparent = true;
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
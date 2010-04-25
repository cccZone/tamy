#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\RendererImpl.h"
#include <string.h>
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

unsigned char Material::s_stagesArrSize = 8;

///////////////////////////////////////////////////////////////////////////////

Material::Material(const std::string& name)
: m_name(name)
, m_lightReflectingProperties(new LightReflectingProperties())
, m_stagesCount(0)
, m_transparent(false)
{
   m_stages = new MaterialStageP[s_stagesArrSize];
   memset(m_stages, NULL, sizeof(MaterialStageP) * s_stagesArrSize);

   checkTransparency();
}

///////////////////////////////////////////////////////////////////////////////

Material::Material(const Material& rhs)
: UniqueObject(rhs)
, m_name(rhs.m_name)
, m_lightReflectingProperties(new LightReflectingProperties(*rhs.m_lightReflectingProperties))
, m_stagesCount(rhs.m_stagesCount)
, m_transparent(false)
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

void Material::setForRendering(RendererImpl& impl) const
{
   impl.setTransparency(m_transparent);
   impl.setLRP(*m_lightReflectingProperties);

   for (unsigned char i = 0; i < m_stagesCount; ++i)
   {
      m_stages[i]->setForRendering(impl);
   }
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
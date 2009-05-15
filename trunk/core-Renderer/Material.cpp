#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\LightReflectingProperties.h"
#include <string.h>
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

unsigned char Material::s_stagesArrSize = 8;

///////////////////////////////////////////////////////////////////////////////

Material::Material(LightReflectingProperties& lrp, 
                   MaterialOperationImplementation& alphaMatOp,
                   MaterialOperationImplementation& colorMatOp,
                   unsigned int index)
      : m_index(index),
      m_lightReflectingProperties(lrp),
      m_disableAlpha(new MaterialOperation(alphaMatOp, MOP_DISABLE, SC_NONE, SC_NONE)),
      m_disableColor(new MaterialOperation(colorMatOp, MOP_DISABLE, SC_NONE, SC_NONE)),
      m_stagesCount(0),
      m_transparent(false)
{
   m_stages = new MaterialStageP[s_stagesArrSize];
   memset(m_stages, NULL, sizeof(MaterialStageP) * s_stagesArrSize);

   checkTransparency();
}

///////////////////////////////////////////////////////////////////////////////

Material::~Material()
{
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

   m_index = 0;
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

bool Material::operator==(const Material& rhs) const
{
   if (m_lightReflectingProperties != rhs.m_lightReflectingProperties) {return false;}

   if (m_stagesCount != rhs.m_stagesCount) {return false;}

   for (unsigned char i = 0; i < m_stagesCount; ++i)
   {
      if (*(m_stages[i]) != *(rhs.m_stages[i])) {return false;}
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool Material::operator!=(const Material& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

bool Material::isTransparent() const
{
   return m_transparent;
}

///////////////////////////////////////////////////////////////////////////////

void Material::setForRendering()
{
   enableTransparency(m_transparent);

   m_lightReflectingProperties.setForRendering();

   for (unsigned char i = 0; i < m_stagesCount; ++i)
   {
      m_stages[i]->setForRendering();
   }

   m_disableAlpha->setForRendering(m_stagesCount);
   m_disableColor->setForRendering(m_stagesCount);
}

///////////////////////////////////////////////////////////////////////////////

void Material::checkTransparency()
{
   m_transparent = false;
   for (unsigned char i = 0; i < m_stagesCount; ++i)
   {
      if (m_stages[i]->isTransparent() == true) 
      {
         m_transparent = true;
         break;
      }
   }

   m_transparent |= m_lightReflectingProperties.isTransparent();
}

///////////////////////////////////////////////////////////////////////////////
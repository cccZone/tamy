#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\MaterialOperation.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

MaterialStage::MaterialStage(Texture& texture,
                             MaterialOperation* colorOp, 
                             MaterialOperation* alphaOp)
      : m_index(-1),
      m_texture(texture),
      m_colorOperation(colorOp),
      m_alphaOperation(alphaOp)
{
   if (colorOp == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a MaterialOperation instance for a color operation");
   }

   if (alphaOp == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a MaterialOperation instance for an alpha operation");
   }
}

///////////////////////////////////////////////////////////////////////////////

MaterialStage::~MaterialStage()
{
   delete m_colorOperation;
   m_colorOperation = NULL;

   delete m_alphaOperation;
   m_alphaOperation = NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool MaterialStage::operator==(const MaterialStage& rhs) const
{
   if (m_texture != rhs.m_texture) {return false;}
   if (*m_colorOperation != *(rhs.m_colorOperation)) {return false;}
   if (*m_alphaOperation != *(rhs.m_alphaOperation)) {return false;}

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool MaterialStage::operator!=(const MaterialStage& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

void MaterialStage::setForRendering()
{
   m_texture.setForRendering(m_index);
   m_colorOperation->setForRendering(m_index);
   m_alphaOperation->setForRendering(m_index);
}

///////////////////////////////////////////////////////////////////////////////

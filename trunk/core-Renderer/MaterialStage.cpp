#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include "core-Renderer\MaterialImpl.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

MaterialStage::MaterialStage(Texture& texture,
                             MaterialOperation* colorOp, 
                             MaterialOperation* alphaOp,
                             CoordinatesOperation* coordsOp)
      : m_index(-1),
      m_texture(texture),
      m_textureImpl(texture.getImpl()),
      m_colorOperation(colorOp),
      m_alphaOperation(alphaOp),
      m_coordsOp(coordsOp)
{
   if (colorOp == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a MaterialOperation instance for a color operation");
   }

   if (alphaOp == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a MaterialOperation instance for an alpha operation");
   }

   if (coordsOp == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a CoordinatesOperation instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

MaterialStage::MaterialStage(const MaterialStage& rhs)
      : m_index(rhs.m_index),
      m_texture(rhs.m_texture),
      m_textureImpl(rhs.m_textureImpl),
      m_colorOperation(new MaterialOperation(*rhs.m_colorOperation)),
      m_alphaOperation(new MaterialOperation(*rhs.m_alphaOperation)),
      m_coordsOp(new CoordinatesOperation(*rhs.m_coordsOp))
{
}

///////////////////////////////////////////////////////////////////////////////

MaterialStage::~MaterialStage()
{
   delete m_coordsOp;
   m_coordsOp = NULL;

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
   if (*m_coordsOp != *(rhs.m_coordsOp)) {return false;}

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool MaterialStage::operator!=(const MaterialStage& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

void MaterialStage::setForRendering(MaterialImpl& impl)
{
   impl.setTexture(m_index, m_textureImpl);
   impl.setColorOperation(m_index, *m_colorOperation);
   impl.setAlphaOperation(m_index, *m_alphaOperation);
   impl.setCoordsOperation(m_index, *m_coordsOp);
}

///////////////////////////////////////////////////////////////////////////////

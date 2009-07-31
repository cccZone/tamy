#pragma once

#include "core-Renderer\MaterialImpl.h"
#include "core-Renderer\LightReflectingProperties.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace RegularTests
{
   class MaterialImplMock : public MaterialImpl
   {
   public:
      void setTransparency(bool enable) {}
      
      void setLRP(LightReflectingProperties& lrp) {}

      void setTexture(unsigned char stageIndex, TextureImpl& textureImpl) {}

      void setColorOperation(unsigned char stageIndex, MaterialOperation& colorOp) {}

      void setAlphaOperation(unsigned char stageIndex, MaterialOperation& alphaOp) {}

      void setCoordsOperation(unsigned char stageIndex, CoordinatesOperation& coordsOp) {}
   };
}

///////////////////////////////////////////////////////////////////////////////

namespace RenderingProcessorTests
{
   class MaterialImplMock : public MaterialImpl
   {
   private:
      std::vector<std::string>& m_log;

   public:
      MaterialImplMock(std::vector<std::string>& log) : m_log(log) {}

      void setTransparency(bool enable) {}
      
      void setLRP(LightReflectingProperties& lrp) 
      {
         char tmpStr[128];
         sprintf_s(tmpStr, 128, "Set material with power = %.1f", lrp.getPower());
         m_log.push_back(tmpStr);
      }

      void setTexture(unsigned char stageIndex, TextureImpl& textureImpl) {}

      void setColorOperation(unsigned char stageIndex, MaterialOperation& colorOp) {}

      void setAlphaOperation(unsigned char stageIndex, MaterialOperation& alphaOp) {}

      void setCoordsOperation(unsigned char stageIndex, CoordinatesOperation& coordsOp) {}
   };
}

///////////////////////////////////////////////////////////////////////////////
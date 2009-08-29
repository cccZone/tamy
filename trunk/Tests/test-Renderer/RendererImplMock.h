#pragma once

#include "core-Renderer\RendererImpl.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\RenderableNode.h"
#include "core-Renderer\Light.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace RegularTests
{
   class RendererImplMock : public RendererImpl
   {
   public:
      void render(Renderable& renderable, RenderingTargetsPolicy& policy) {}

      void enableLights(const Array<Light*>& lights) {}

      void disableLights(const Array<Light*>& lights) {}

      void setMatrices(const D3DXMATRIX& viewMtx, const D3DXMATRIX& projMtx) {}

      void setObjectMatrices(const Array<D3DXMATRIX>& matrices) {}

      void setTransparency(bool enable) {}
      
      void setLRP(LightReflectingProperties& lrp) {}

      void setTexture(unsigned char stageIndex, Texture& textureImpl) {}

      void setColorOperation(unsigned char stageIndex, MaterialOperation& colorOp) {}

      void setAlphaOperation(unsigned char stageIndex, MaterialOperation& alphaOp) {}

      void setCoordsOperation(unsigned char stageIndex, CoordinatesOperation& coordsOp) {}
   };
}

///////////////////////////////////////////////////////////////////////////////

namespace CallSeqTrack
{
   class RendererImplMock : public RendererImpl
   {
   private:
      std::vector<std::string>& m_log;

   public:
      RendererImplMock(std::vector<std::string>& log) : m_log(log) {}

      void render(Renderable& renderable, RenderingTargetsPolicy& policy)
      {
         m_log.push_back("Rendering a renderable...");
      }

      void enableLights(const Array<Light*>& lights)
      {
         std::string msg = "Enabling lights: ";

         unsigned int lightsCount = lights.size();
         for (unsigned int i = 0; i < lightsCount; ++i)
         {
            msg += lights[i]->getName();
            if (i < (lightsCount - 1)) {msg += ", ";}
         }

         m_log.push_back(msg);
      }

      void disableLights(const Array<Light*>& lights)
      {
         std::string msg = "Disabling lights: ";

         unsigned int lightsCount = lights.size();
         for (unsigned int i = 0; i < lightsCount; ++i)
         {
            msg += lights[i]->getName();
            if (i < (lightsCount - 1)) {msg += ", ";}
         }

         m_log.push_back(msg);
      }

      void setMatrices(const D3DXMATRIX& viewMtx, const D3DXMATRIX& projMtx)
      {
         m_log.push_back("Set view & proj matrices");
      }

      void setObjectMatrices(const Array<D3DXMATRIX>& matrices)
      {
         m_log.push_back("Setting object matrices");
      }

      void setTransparency(bool enable) {}
      
      void setLRP(LightReflectingProperties& lrp) 
      {
         char tmpStr[128];
         sprintf_s(tmpStr, 128, "Set material with power = %.1f", lrp.getPower());
         m_log.push_back(tmpStr);
      }

      void setTexture(unsigned char stageIndex, Texture& textureImpl) {}

      void setColorOperation(unsigned char stageIndex, MaterialOperation& colorOp) {}

      void setAlphaOperation(unsigned char stageIndex, MaterialOperation& alphaOp) {}

      void setCoordsOperation(unsigned char stageIndex, CoordinatesOperation& coordsOp) {}
   };
}

///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "core-Renderer\Renderer.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace RegularTests
{
   class RendererImplementationMock : public Renderer
   {
   protected:
      void resetViewport(unsigned int width, unsigned int height) {}

      void renderingBegin() {}

      void renderingEnd() {}

      bool isGraphicsSystemReady() const {return true;}

      void attemptToRecoverGraphicsSystem() {}
   };
}

///////////////////////////////////////////////////////////////////////////////

namespace RendererTests
{
   class RendererImplementationMock : public Renderer
   {
   private:
      std::vector<std::string>& m_seqLog;
      bool m_deviceReady;

   public:
      RendererImplementationMock(std::vector<std::string>& seqLog) 
            : m_seqLog(seqLog),
            m_deviceReady(true)
      {
      }

      void setDeviceReady(bool enable) 
      {
         m_deviceReady = enable;
      }


   protected:
      void resetViewport(unsigned int width, unsigned int height) {m_seqLog.push_back("Renderer - resetViewport");}

      void renderingBegin() {m_seqLog.push_back("Renderer - begin");}

      void renderingEnd() {m_seqLog.push_back("Renderer - end");}

      bool isGraphicsSystemReady() const {return m_deviceReady;}

      void attemptToRecoverGraphicsSystem() 
      {
         m_seqLog.push_back("Renderer - attemptToRecoverGraphicsSystem");
         m_deviceReady = true;
      }
   };
}

///////////////////////////////////////////////////////////////////////////////

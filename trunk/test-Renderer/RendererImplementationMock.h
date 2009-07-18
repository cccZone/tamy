#pragma once

#include "core-Renderer\Renderer.h"
#include "core-Renderer\GraphicalNode.h"
#include <d3dx9.h>
#include "core-Renderer\Camera.h"
#include "core-Renderer\DrawingPass.h"
#include "RenderingTargetMock.h"


///////////////////////////////////////////////////////////////////////////////

class RendererImplementationMock : public Renderer
{
private:
   RenderingTargetMock m_renderingTarget;

   bool m_deviceReady;
   D3DXMATRIX m_viewMtx;
   UINT m_maxLightsCount;
   bool m_deviceRecoveryAttemptMade;
   bool m_viewportReset;
   bool m_presentCalled;

public:
   RendererImplementationMock(bool addDefaultRenderingTarget = true) 
         : Renderer(1),
         m_deviceReady(true),
         m_maxLightsCount(255),
         m_deviceRecoveryAttemptMade(false),
         m_viewportReset(false),
         m_presentCalled(false)
   {
      D3DXMatrixIdentity(&m_viewMtx);
      addPass(new DrawingPass());

      if (addDefaultRenderingTarget)
      {
         addRenderingTarget(m_renderingTarget);
      }
   }

   void setDeviceReady(bool enable) {m_deviceReady = enable;}

   const D3DXMATRIX& getViewMatrixSet() const {return m_viewMtx;}

   void setMaxLightsCount(UINT val) {m_maxLightsCount = val;}

   bool wasDeviceRecoveryAttemptMade() 
   {
      bool result = m_deviceRecoveryAttemptMade;
      m_deviceRecoveryAttemptMade = false;
      return result;
   }

   bool wasViewportReset()
   {
      bool result = m_viewportReset;
      m_viewportReset = false;
      return result;
   }

   bool wasPresentCalled()
   {
      bool result = m_presentCalled;
      m_presentCalled = false;
      return result;
   }

protected:

   void resetViewport(unsigned int width, unsigned int height) {m_viewportReset = true;}

   UINT getMaxLightsCount() const {return m_maxLightsCount;}

   void setViewMatrix(const D3DXMATRIX& mtx) {m_viewMtx = mtx;}

   void setProjectionMatrix(const D3DXMATRIX& mtx) {}

   void renderingBegin() {}

   void renderingEnd() {m_presentCalled = true;}

   bool isGraphicsSystemReady() const {return m_deviceReady;}

   void attemptToRecoverGraphicsSystem() 
   {
      m_deviceRecoveryAttemptMade = true;
      m_deviceReady = true;
   }
};

///////////////////////////////////////////////////////////////////////////////

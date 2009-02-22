#pragma once

#include "Renderer.h"
#include "GraphicalNode.h"
#include <d3dx9.h>
#include "Camera.h"


///////////////////////////////////////////////////////////////////////////////

class RendererImplementationMock : public Renderer
{
private:
   bool m_deviceReady;
   D3DXMATRIX m_viewMtx;
   Camera m_dummyCamera;
   UINT m_maxLightsCount;
   bool m_deviceRecoveryAttemptMade;
   bool m_viewportReset;
   bool m_initialStateSet;
   bool m_presentCalled;

public:
   RendererImplementationMock() 
         : m_deviceReady(true),
         m_dummyCamera("dummyCamera"),
         m_maxLightsCount(255),
         m_deviceRecoveryAttemptMade(false),
         m_viewportReset(false),
         m_initialStateSet(false),
         m_presentCalled(false)
   {
      D3DXMatrixIdentity(&m_viewMtx);
      setActiveCamera(m_dummyCamera);
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

   bool wasInitialStateSet()
   {
      bool result = m_initialStateSet;
      m_initialStateSet = false;
      return result;
   }

   bool wasPresentCalled()
   {
      bool result = m_presentCalled;
      m_presentCalled = false;
      return result;
   }

protected:
   void initRenderer() {m_initialStateSet = true;}

   void resetViewport(unsigned int width, unsigned int height) {m_viewportReset = true;}

   UINT getMaxLightsCount() const {return m_maxLightsCount;}

   void setViewMatrix(const D3DXMATRIX& mtx) {m_viewMtx = mtx;}

   void setProjectionMatrix(const D3DXMATRIX& mtx) {}

   void executeRenderingCommands(const std::list<RenderingCommand>& commands) 
   {
      for (std::list<RenderingCommand>::const_iterator it = commands.begin();
         it != commands.end(); ++it)
      {
         (*it)();
      }
   }

   void present() {m_presentCalled = true;}

   bool isGraphicsSystemReady() const {return m_deviceReady;}

   void attemptToRecoverGraphicsSystem() 
   {
      m_deviceRecoveryAttemptMade = true;
      m_deviceReady = true;
   }
};

///////////////////////////////////////////////////////////////////////////////

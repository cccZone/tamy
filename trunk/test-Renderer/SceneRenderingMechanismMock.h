#pragma once

#include "core-Renderer\SceneRenderingMechanism.h"
#include "core-Renderer\GraphicalNode.h"
#include <d3dx9.h>
#include "core-Renderer\Camera.h"
#include "RenderingTargetsPolicyMock.h"


///////////////////////////////////////////////////////////////////////////////

namespace RegularTests
{
   class SceneRenderingMechanismMock : public SceneRenderingMechanism
   {
   private:
      D3DXMATRIX m_viewMtx;

   public:
      SceneRenderingMechanismMock(unsigned int maxLightsCount = 255) 
            : SceneRenderingMechanism(new RenderingTargetsPolicyMock(), maxLightsCount)
      {
         D3DXMatrixIdentity(&m_viewMtx);
      }

      const D3DXMATRIX& getViewMatrixSet() const {return m_viewMtx;}

   protected:
      void setViewMatrix(const D3DXMATRIX& mtx) {m_viewMtx = mtx;}

      void setProjectionMatrix(const D3DXMATRIX& mtx) {}
   };
}

///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ext-MotionControllers\WaypointCameraController.h"
#include "core\NodeAction.h"


///////////////////////////////////////////////////////////////////////////////

class JumpToNodeAction : public NodeAction
{
private:
   WaypointCameraController& m_camController;
   int& m_pointIdx;

public:
   JumpToNodeAction(WaypointCameraController& camController,
                    int& pointIdx)
         : m_camController(camController),
         m_pointIdx(pointIdx)
   {}

   void performNodeAction()
   {
      m_pointIdx = (m_pointIdx + 1) % 3;
      m_camController.goTo(m_pointIdx);
   }
};

///////////////////////////////////////////////////////////////////////////////

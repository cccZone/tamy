#include "ext-MotionControllers\WaypointCameraController.h"
#include "ext-MotionControllers\TimeFunction.h"
#include "core\Assert.h"
#include "core\MatrixInterpolator.h"
#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

WaypointCameraController::WaypointCameraController(Node& camera,
                                                   const D3DXVECTOR3& observationPointOffset,
                                                   TimeFunction* m_timeFunction)
      : m_camera(camera),
      m_observationPointOffset(observationPointOffset),
      m_mtxInterpolator(new MatrixInterpolator()),
      m_timeFunction(m_timeFunction),
      m_advancement(0)
{
   m_start = m_dest = m_camera.getGlobalMtx();
}

///////////////////////////////////////////////////////////////////////////////

WaypointCameraController::~WaypointCameraController()
{
   delete m_mtxInterpolator;
   m_mtxInterpolator = NULL;

   delete m_timeFunction;
   m_timeFunction = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void WaypointCameraController::registerWaypoint(int waypointIdx, Node& waypoint)
{
   while (waypointIdx >= (int)m_waypoints.size())
   {
      m_waypoints.push_back(NULL);
   }

   m_waypoints[waypointIdx] = &waypoint;
}

///////////////////////////////////////////////////////////////////////////////

void WaypointCameraController::goTo(int waypointIdx)
{
   ASSERT_MSG(waypointIdx < (int)m_waypoints.size(), "Waypoint index out of range");
   ASSERT_MSG(m_waypoints[waypointIdx] != NULL, "Waypoint doesn't exist");

   D3DXMATRIX destination = m_waypoints[waypointIdx]->getGlobalMtx();
   D3DXVECTOR3 lookVec;
   D3DXVec3TransformCoord(&lookVec, &m_observationPointOffset, &destination);

   destination._41 = lookVec.x;
   destination._42 = lookVec.y;
   destination._43 = lookVec.z;

   m_start = m_camera.getLocalMtx();
   m_dest = destination;
   m_advancement = 0;
}

///////////////////////////////////////////////////////////////////////////////

void WaypointCameraController::update(float timeElapsed)
{
   m_advancement += timeElapsed;
   if (m_advancement > 1) {m_advancement = 1;}

   D3DXMATRIX destination = (*m_mtxInterpolator)(m_start, m_dest, (*m_timeFunction)(m_advancement));
   m_camera.setLocalMtx(destination);
}

///////////////////////////////////////////////////////////////////////////////

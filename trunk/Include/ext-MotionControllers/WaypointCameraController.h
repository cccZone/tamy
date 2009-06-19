#pragma once

#include <vector>
#include <d3dx9.h>
#include "core\Node.h"
#include "core\Node.h"
#include "core\Assert.h"
#include "core\MatrixInterpolator.h"


///////////////////////////////////////////////////////////////////////////////

template<typename TimeFunction>
class WaypointCameraController
{
private:
   Node& m_camera;
   D3DXVECTOR3 m_observationPointOffset;
   MatrixInterpolator* m_mtxInterpolator;
   TimeFunction* m_timeFunction;
   std::vector<Node*> m_waypoints;

   D3DXMATRIX m_start;
   D3DXMATRIX m_dest;
   float m_advancement;

public:
   WaypointCameraController(Node& camera,
                            const D3DXVECTOR3& observationPointOffset)
         : m_camera(camera),
         m_observationPointOffset(observationPointOffset),
         m_mtxInterpolator(new MatrixInterpolator()),
         m_timeFunction(new TimeFunction()),
         m_advancement(0)
   {
      m_start = m_dest = m_camera.getGlobalMtx();
   }

   ~WaypointCameraController()
   {
      delete m_mtxInterpolator;
      m_mtxInterpolator = NULL;

      delete m_timeFunction;
      m_timeFunction = NULL;
   }

   void registerWaypoint(int waypointIdx, Node& waypoint)
   {
      while (waypointIdx >= m_waypoints.size())
      {
         m_waypoints.push_back(NULL);
      }

      m_waypoints[waypointIdx] = &waypoint;
   }


   void goTo(int waypointIdx)
   {
      ASSERT(waypointIdx < m_waypoints.size(), "Waypoint index out of range");
      ASSERT(m_waypoints[waypointIdx] != NULL, "Waypoint doesn't exist");

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

   const D3DXMATRIX& getDestination() const {return m_dest;}

   void update(float timeElapsed)
   {
      m_advancement += timeElapsed;
      if (m_advancement > 1) {m_advancement = 1;}

      D3DXMATRIX destination = (*m_mtxInterpolator)(m_start, m_dest, (*m_timeFunction)(m_advancement));
      m_camera.setLocalMtx(destination);
   }
};

///////////////////////////////////////////////////////////////////////////////

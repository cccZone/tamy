#pragma once

#include <vector>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Node;
class MatrixInterpolator;
class TimeFunction;

///////////////////////////////////////////////////////////////////////////////

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
                            const D3DXVECTOR3& observationPointOffset,
                            TimeFunction* m_timeFunction);

   ~WaypointCameraController();

   void registerWaypoint(int waypointIdx, Node& waypoint);

   void goTo(int waypointIdx);

   const D3DXMATRIX& getDestination() const {return m_dest;}

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

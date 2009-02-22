#pragma once

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class is responsible for controlng the transformation
 * of a node.
 * All the methods operate in the GLOBAL COORDINATE SYSTEM,
 * (as opposed to the node's local coordinate system),
 * allowing the user of this class to take control of the
 * node and move it around the scene
 */
class UnconstrainedMotionController
{
private:
   Node& m_controlledNode;

public:
   UnconstrainedMotionController(Node& controlledNode);

   void move(const D3DXVECTOR3& transVec);
   void rotate(float pitch, float yaw, float roll);

   void setPosition(const D3DXVECTOR3& vec);
   D3DXVECTOR3 getRightVec() const;
   D3DXVECTOR3 getUpVec() const;
   D3DXVECTOR3 getLookVec() const;
   D3DXVECTOR3 getPosition() const;

private:
   void regenerateVectors(D3DXVECTOR3& rightVec, D3DXVECTOR3& upVec, D3DXVECTOR3& lookVec) const;
};

///////////////////////////////////////////////////////////////////////////////

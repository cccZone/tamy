/// @file   ext-MotionControllers/UnconstrainedMotionController.h
/// @brief  spaceship-like movement for the node
#pragma once

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class is responsible for controlling the transformation
 * of a node.
 * All the methods operate in the GLOBAL COORDINATE SYSTEM,
 * (as opposed to the node's local coordinate system),
 * allowing the user of this class to take control of the
 * node and move it around the scene
 */
class UnconstrainedMotionController
{
private:
   Node&    m_controlledNode;

public:
   /**
    * Constructor.
    *
    * @param controlledNode
    */
   UnconstrainedMotionController( Node& controlledNode );

   void move( const D3DXVECTOR3& transVec );

   /**
    * 3D rotation.
    */
   void rotate( float pitch, float yaw, float roll );

   /**
    * 2D rotation around global Y axis
    */
   void rotate( float pitch, float yaw );

   void setPosition( const D3DXVECTOR3& vec );
   D3DXVECTOR3 getRightVec() const;
   D3DXVECTOR3 getUpVec() const;
   D3DXVECTOR3 getLookVec() const;
   D3DXVECTOR3 getPosition() const;

private:
   void regenerateVectors( D3DXVECTOR3& rightVec, D3DXVECTOR3& upVec, D3DXVECTOR3& lookVec ) const;
};

///////////////////////////////////////////////////////////////////////////////

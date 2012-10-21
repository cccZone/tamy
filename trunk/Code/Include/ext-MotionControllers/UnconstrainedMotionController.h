/// @file   ext-MotionControllers/UnconstrainedMotionController.h
/// @brief  spaceship-like movement for the node
#pragma once

#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

class Node;
struct Vector;

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
   DECLARE_ALLOCATOR( UnconstrainedMotionController, AM_DEFAULT );

private:
   Node&    m_controlledNode;

public:
   /**
    * Constructor.
    *
    * @param controlledNode
    */
   UnconstrainedMotionController( Node& controlledNode );

   void move( const Vector& transVec );

   /**
    * 3D rotation.
    */
   void rotate( float pitch, float yaw, float roll );

   /**
    * 2D rotation around global Y axis
    */
   void rotate( float pitch, float yaw );

   void setPosition( const Vector& vec );
   const Vector& getRightVec() const;
   const Vector& getUpVec() const;
   const Vector& getLookVec() const;
   const Vector& getPosition() const;

private:
   void regenerateVectors( Vector& rightVec, Vector& upVec, Vector& lookVec ) const;
};

///////////////////////////////////////////////////////////////////////////////

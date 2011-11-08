/// @file   TamyEditor/NodeTransformController.h
/// @brief  translates the manipulated node
#pragma once

#include "SceneRendererInputController.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class UserInputController;
class Renderer;
class Camera;
class SpatialEntity;
class Node;

///////////////////////////////////////////////////////////////////////////////

enum NodeTransformControlMode
{
   NTM_TRANSLATE,
   NTM_ROTATE
};

///////////////////////////////////////////////////////////////////////////////

class NodeTransformController : public SceneRendererInputController
{
private:
   enum MovementDirection
   {
      MD_LEFT,
      MD_RIGHT,
      MD_FRONT,
      MD_BACK
   };

private:
   UserInputController*                   m_uic;
   const Renderer*                        m_renderer;
   const Camera*                          m_camera;
   std::vector< SpatialEntity* >          m_nodes;

   bool                                   m_movementDir[4];
   D3DXVECTOR3                            m_rotationAxis;

   NodeTransformControlMode               m_controlMode;

public:
   /**
    * Constructor.
    *
    * @param nodes    controlled nodes
    * @param mode     control mode
    */
   NodeTransformController( const std::vector< SpatialEntity* >& nodes, NodeTransformControlMode mode );
   ~NodeTransformController();

   // -------------------------------------------------------------------------
   // SceneRendererInputController implementation
   // -------------------------------------------------------------------------
   void initialize( TamySceneWidget& widget );

   // -------------------------------------------------------------------------
   // TimeDependent implementation
   // -------------------------------------------------------------------------
   void update( float timeElapsed );

   // -------------------------------------------------------------------------
   // KeyStatusHandler implementation
   // -------------------------------------------------------------------------
   void keySmashed( unsigned char keyCode );
   void keyHeld( unsigned char keyCode );
   void keyReleased( unsigned char keyCode );

private:
   void transformNode( Node& node, const D3DXVECTOR2& valChange ) const;
   D3DXVECTOR3 selectGlobalRotationAxis() const;
};

///////////////////////////////////////////////////////////////////////////////

/// @file   TamyEditor/CameraPlaneNodeManipulator.h
/// @brief  manipulates the selected objects moving them on the active camera's near plane
#pragma once

#include "SceneRendererInputController.h"
#include "core\Vector.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class UserInputController;
class Renderer;
class Camera;
class SpatialEntity;
class Node;
class GizmoAxis;
class Matrix;

///////////////////////////////////////////////////////////////////////////////

/**
 * Manipulates the selected objects moving them on the active camera's near plane.
 */
class CameraPlaneNodeManipulator : public SceneRendererInputController
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
   TamySceneWidget*                       m_sceneWidget;
   const Renderer*                        m_renderer;
   const Camera*                          m_camera;
   std::vector< SpatialEntity* >          m_selectedNodes;

   bool                                   m_movementDir[4];

public:
   /**
    * Constructor.
    *
    * @param selectedNodes
    */
   CameraPlaneNodeManipulator( const std::vector< SpatialEntity* >& selectedNodes );
   ~CameraPlaneNodeManipulator();

   // -------------------------------------------------------------------------
   // SceneRendererInputController implementation
   // -------------------------------------------------------------------------
   void initialize( TamySceneWidget& widget );

   // -------------------------------------------------------------------------
   // TimeDependent implementation
   // -------------------------------------------------------------------------
   void update( float timeElapsed );

private:
   void translateSelectedNodes( const Vector& valChange ) const;
   void rotateSelectedNodes( const Vector& valChange ) const;
};

///////////////////////////////////////////////////////////////////////////////

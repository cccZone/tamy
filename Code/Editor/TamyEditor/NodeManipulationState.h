/// @file      TamyEditor/NodeManipulationState.h
/// @brief     SceneObjectsManipulator node manipulation state
#pragma once

#include "core-AI/FSMState.h"
#include "SceneObjectsManipulator.h"


///////////////////////////////////////////////////////////////////////////////

enum NodeTransformControlMode;
class GizmoAxis;
class UserInputController;
class TamySceneWidget;

///////////////////////////////////////////////////////////////////////////////

/**
 * SceneObjectsManipulator node manipulation state.
 */
class NodeManipulationState : public FSMState< SceneObjectsManipulator >
{
   DECLARE_CLASS()

private:
   GizmoAxis*                 m_gizmoAxis;
   UserInputController*       m_uic;

public:
   /**
    * Constructor.
    */
   NodeManipulationState();

   /**
    * Initializes the input controller this state uses to check the user's input.
    *
    * @param widget
    */
   void initInput( TamySceneWidget& widget );

   /**
    * Sets the gizmo axis we will use to manipulate the object.
    *
    * @param selectedGizmoAxis
    */
   void setGizmoAxis( GizmoAxis& selectedGizmoAxis );

   // ----------------------------------------------------------------------
   // FSMState implementation
   // ----------------------------------------------------------------------
   void activate();
   void deactivate();
   void execute( float timeElapsed );

private:
   void setupController();
};

///////////////////////////////////////////////////////////////////////////////

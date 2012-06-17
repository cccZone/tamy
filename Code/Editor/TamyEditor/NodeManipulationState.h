/// @file      TamyEditor/NodeManipulationState.h
/// @brief     SceneObjectsManipulator node manipulation state
#pragma once

#include "core-AI/FSMState.h"
#include "SceneObjectsManipulator.h"
#include "SceneObjectsManipulatorState.h"


///////////////////////////////////////////////////////////////////////////////

enum NodeTransformControlMode;
class GizmoAxis;

///////////////////////////////////////////////////////////////////////////////

/**
 * SceneObjectsManipulator node manipulation state.
 */
class NodeManipulationState : public FSMState< SceneObjectsManipulator >, public SceneObjectsManipulatorState
{
   DECLARE_CLASS()

private:
   GizmoAxis*        m_gizmoAxis;

public:
   /**
    * Constructor.
    */
   NodeManipulationState();

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

   // ----------------------------------------------------------------------
   // SceneObjectsManipulatorState implementation
   // ----------------------------------------------------------------------
   bool keySmashed( unsigned char keyCode );
   bool keyHeld( unsigned char keyCode );
   bool keyReleased( unsigned char keyCode );

private:
   void setupController();
};

///////////////////////////////////////////////////////////////////////////////

/// @file      TamyEditor/NodeManipulationState.h
/// @brief     SceneObjectsManipulator node manipulation state
#pragma once

#include "core-AI/FSMState.h"
#include "SceneObjectsManipulator.h"
#include "SceneObjectsManipulatorState.h"


///////////////////////////////////////////////////////////////////////////////

enum NodeTransformControlMode;

///////////////////////////////////////////////////////////////////////////////

/**
 * SceneObjectsManipulator node manipulation state.
 */
class NodeManipulationState : public FSMState< SceneObjectsManipulator >, public SceneObjectsManipulatorState
{
   DECLARE_CLASS()

public:
   // ----------------------------------------------------------------------
   // FSMState implementation
   // ----------------------------------------------------------------------
   void activate();
   void deactivate();

   // ----------------------------------------------------------------------
   // SceneObjectsManipulatorState implementation
   // ----------------------------------------------------------------------
   void onSettingsChanged();
   bool keySmashed( unsigned char keyCode );
   bool keyHeld( unsigned char keyCode );
   bool keyReleased( unsigned char keyCode );

private:
   void setupController();
};

///////////////////////////////////////////////////////////////////////////////

/// @file      TamyEditor/NavigationState.h
/// @brief     SceneObjectsManipulator navigation state
#pragma once

#include "core-AI/FSMState.h"
#include "SceneObjectsManipulator.h"
#include "SceneObjectsManipulatorState.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * SceneObjectsManipulator navigation state.
 */
class NavigationState : public FSMState< SceneObjectsManipulator >, public SceneObjectsManipulatorState
{
public:
   DECLARE_RTTI_CLASS

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
};

///////////////////////////////////////////////////////////////////////////////

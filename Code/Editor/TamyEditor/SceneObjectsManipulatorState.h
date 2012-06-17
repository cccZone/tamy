/// @file   TamyEditor/SceneObjectsManipulatorState.h
/// @brief  SceneObjectsManipulator state interface
#pragma once

#include "core-AI/FSMState.h"
#include "SceneObjectsManipulator.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * SceneObjectsManipulator state interface.
 */ 
class SceneObjectsManipulatorState
{
public:
   virtual ~SceneObjectsManipulatorState() {}

   /**
    * Called when a key gets smashed.
    *
    * @param keyCode
    */
   virtual bool keySmashed( unsigned char keyCode ) = 0;

   /**
    * Called when a key gets held.
    *
    * @param keyCode
    */
   virtual bool keyHeld( unsigned char keyCode ) = 0;

   /**
    * Called when a key gets released.
    *
    * @param keyCode
    */
   virtual bool keyReleased( unsigned char keyCode ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

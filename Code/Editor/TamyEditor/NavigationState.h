/// @file      TamyEditor/NavigationState.h
/// @brief     SceneObjectsManipulator navigation state
#pragma once

#include "core-AI/FSMState.h"
#include "SceneObjectsManipulator.h"
#include "SceneObjectsManipulatorState.h"
#include "SceneQuery.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * SceneObjectsManipulator navigation state.
 */
class NavigationState : public FSMState< SceneObjectsManipulator >, public SceneObjectsManipulatorState, public SceneQuery
{
public:
   DECLARE_RTTI_CLASS

private:
   D3DXVECTOR2          m_queryPos;

public:
   // ----------------------------------------------------------------------
   // SceneQuery implementation
   // ----------------------------------------------------------------------
   const D3DXVECTOR2& getQueriedPosition() const { return m_queryPos; }
   void setResult( Entity* foundEntity );

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

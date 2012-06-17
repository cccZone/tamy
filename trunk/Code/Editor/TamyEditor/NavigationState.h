/// @file      TamyEditor/NavigationState.h
/// @brief     SceneObjectsManipulator navigation state
#pragma once

#include "core-AI/FSMState.h"
#include "SceneObjectsManipulator.h"
#include "SceneObjectsManipulatorState.h"
#include "SceneQuery.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * SceneObjectsManipulator navigation state.
 *
 * Allows to select objects by pressing Left Mouse Button twice on
 * the desired object or to switch to manipulation mode by pressing
 * and holding the Left Mouse Button and moving the mouse around.
 */
class NavigationState : public FSMState< SceneObjectsManipulator >, public SceneObjectsManipulatorState, public SceneQuery
{
public:
   DECLARE_CLASS()

private:
   Vector               m_queryPos;

public:
   // ----------------------------------------------------------------------
   // SceneQuery implementation
   // ----------------------------------------------------------------------
   const Vector& getQueriedPosition() const { return m_queryPos; }
   void setResult( const Array< Entity* >& foundEntities );

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
   void analyzeSelectedNodes( const Array< SpatialEntity* >& selectedNodes );
};

///////////////////////////////////////////////////////////////////////////////

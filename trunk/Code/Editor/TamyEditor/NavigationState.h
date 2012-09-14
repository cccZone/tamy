/// @file      TamyEditor/NavigationState.h
/// @brief     SceneObjectsManipulator navigation state
#pragma once

#include "core-AI/FSMState.h"
#include "SceneObjectsManipulator.h"
#include "SceneQuery.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

class UserInputController;

///////////////////////////////////////////////////////////////////////////////

/**
 * SceneObjectsManipulator navigation state.
 *
 * Allows to select objects by pressing Left Mouse Button twice on
 * the desired object or to switch to manipulation mode by pressing
 * and holding the Left Mouse Button and moving the mouse around.
 */
class NavigationState : public FSMState< SceneObjectsManipulator >, public SceneQuery
{
public:
   DECLARE_CLASS()

private:
   UserInputController*       m_uic;
   Vector                     m_queryPos;
   bool                       m_waitingForQueryResults;

public:
   /**
    * Constructor.
    */
   NavigationState();

   /**
    * Initializes the input controller this state uses to check the user's input.
    *
    * @param widget
    */
   void initInput( TamySceneWidget& widget );

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
   void execute( float timeElapsed );

private:
   void commitSelectdEntities( const Array< Entity* >& selectedEntities );
};

///////////////////////////////////////////////////////////////////////////////

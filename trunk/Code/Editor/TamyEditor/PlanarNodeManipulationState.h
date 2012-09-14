/// @file      TamyEditor/GizmoNodeManipulationState.h
/// @brief     SceneObjectsManipulator node manipulation state that manipulates nodes in the camera's front plane ( in the World space of course )
#pragma once

#include "core-AI/FSMState.h"
#include "SceneObjectsManipulator.h"


///////////////////////////////////////////////////////////////////////////////

enum NodeTransformControlMode;
class UserInputController;
class TamySceneWidget;

///////////////////////////////////////////////////////////////////////////////

/**
 * SceneObjectsManipulator node manipulation state that manipulates nodes
 * in the camera's front plane ( in the World space of course ).
 */
class PlanarNodeManipulationState : public FSMState< SceneObjectsManipulator >
{
   DECLARE_CLASS()

private:
   UserInputController*       m_uic;

public:
   /**
    * Constructor.
    */
   PlanarNodeManipulationState();

   /**
    * Initializes the input controller this state uses to check the user's input.
    *
    * @param widget
    */
   void initInput( TamySceneWidget& widget );

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

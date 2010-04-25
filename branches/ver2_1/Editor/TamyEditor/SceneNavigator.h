#pragma once

/// @file   TamyEditor\SceneNavigator.h
/// @brief  scene navigation component

#include "core\Component.h"


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class UnconstrainedMotionController;

///////////////////////////////////////////////////////////////////////////////

/**
 * This component is responsible for navigating scene.
 */
class SceneNavigator : public Component<TamyEditor>
{
private:
   UnconstrainedMotionController* m_cameraController;

public:
   /**
    * Constructor.
    */
   SceneNavigator();
   ~SceneNavigator();

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize(TamyEditor& mgr);
};

///////////////////////////////////////////////////////////////////////////////

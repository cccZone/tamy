/// @file   TamyEditor/SceneRendererInputController.h
/// @brief  the scene rendering widget input controller
#pragma once

#include "core-AppFlow.h"


///////////////////////////////////////////////////////////////////////////////

class TamySceneWidget;

///////////////////////////////////////////////////////////////////////////////

class SceneRendererInputController : public KeyStatusHandler, public TimeDependent
{
public:
   virtual ~SceneRendererInputController() {}

   /**
    * Initialize the controller - this will be called by the renderer
    * once the controller is set in ti.
    *
    * @param widget.
    */
   virtual void initialize( TamySceneWidget& widget ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

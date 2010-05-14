#pragma once

/// @file   TamyEditor\SceneEditor.h
/// @brief  Simple editor for opening a new scene in the editor

#include "ResourceEditor.h"


///////////////////////////////////////////////////////////////////////////////

class Model;

///////////////////////////////////////////////////////////////////////////////

class SceneEditor : public ResourceEditor
{
private:
   Model&      m_scene;

public:
   /**
    * Constructor.
    *
    * @param scene      scene to be edited
    */
   SceneEditor( Model& scene );

   // -------------------------------------------------------------------------
   // ResourceEditor implementation
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );
};

///////////////////////////////////////////////////////////////////////////////

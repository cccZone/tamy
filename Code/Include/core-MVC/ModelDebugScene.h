/// @file   core-MVC/DebugScene.h
/// @brief  a debug scene view
#pragma once

#include "core-MVC/ModelView.h"
#include <set>


///////////////////////////////////////////////////////////////////////////////

class DebugScene;
class IDebugDrawable;

///////////////////////////////////////////////////////////////////////////////

/**
 * A composite capable of holding many debug drawables and exposing them 
 * to the host mechanism as a single instance.
 */
class ModelDebugScene : public ModelView
{
private:
   DebugScene&                   m_debugScene;
   std::set< IDebugDrawable* >   m_entities;

public:
   ModelDebugScene( DebugScene& debugScene );
   ~ModelDebugScene();

   // -------------------------------------------------------------------------
   // ModelView implementation
   // -------------------------------------------------------------------------
   virtual void onEntityAdded( Entity& entity );
   virtual void onEntityRemoved( Entity& entity );
   virtual void onEntityChanged( Entity& entity );
   virtual void resetContents();
};

///////////////////////////////////////////////////////////////////////////////

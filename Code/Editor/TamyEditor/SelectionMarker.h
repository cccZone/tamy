#pragma once

/// @file   TamyEditor\SelectionMarker.h
/// @brief  an effect visualizing the selected object

#include "core-Renderer\ShaderEffect.h"


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;
class Camera;
class Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * An effect rendering a selection hull around an entity.
 */
class SelectionMarker : public ShaderEffect
{
   DECLARE_CLASS(SelectionMarker)

private:
   Camera* m_camera;
   Node* m_renderedNode;

public:
   SelectionMarker();

   /**
    * Factory method creating an instance of this class.
    */
   static Effect* create( ResourcesManager& rm );

protected:
   void onBeginRendering();
   void onAttached(Entity& parent);
   void onDetached(Entity& parent);
   void onAttached(Model& hostModel);
   void onDetached(Model& hostModel);
   void onComponentAdded( Component< Model >& component );
};


///////////////////////////////////////////////////////////////////////////////

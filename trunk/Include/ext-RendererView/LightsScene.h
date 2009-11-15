#pragma once

/// @file   ext-RendererView\LightScene.h
/// @brief  a light storage implementation

#include "core-Renderer\LightsStorage.h"
#include "core\LinearStorage.h"


///////////////////////////////////////////////////////////////////////////////

class Light;
class Node;

///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

/** 
 * A simple adapter to a linear storage of lights that can be used
 * as an input to a RenderLights action.
 */
class LightsScene : public LightsStorage
{
private:
   LinearStorage<Light> m_storage;

public:
   void add(Light& light);
   void remove(Light& light);

   void query(const BoundingVolume& volume, Array<Light*>& outLights);
};

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer

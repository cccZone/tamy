#pragma once

/// @file   ext-Demo\LightScene.h
/// @brief  a light storage implementation for demo purposes

#include "core-Renderer\LightsStorage.h"
#include "core\LinearStorage.h"


///////////////////////////////////////////////////////////////////////////////

class Light;
class Node;

///////////////////////////////////////////////////////////////////////////////

namespace demo
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
   Node* m_root;

public:
   LightsScene();
   ~LightsScene();

   void insert(Light* light);

   void query(const D3DXVECTOR3& point, Array<Light*>& outLights);
};

///////////////////////////////////////////////////////////////////////////////

} // namespace demo

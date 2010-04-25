#include "ext-RendererView\LightsScene.h"
#include "core\PointVolume.h"
#include "core\Node.h"
#include "core-Renderer\Light.h"


namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

void LightsScene::add(Light& light)
{
   m_storage.insert(light);
}

///////////////////////////////////////////////////////////////////////////////

void LightsScene::remove(Light& light)
{
   m_storage.remove(light);
}

///////////////////////////////////////////////////////////////////////////////

void LightsScene::query(const BoundingVolume& volume, Array<Light*>& outLights)
{
   m_storage.query(volume, outLights);
}

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer

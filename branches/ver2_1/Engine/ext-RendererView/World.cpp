#include "ext-RendererView\World.h"
#include "ext-RendererView\RendererView.h"
#include "core-Scene\WorldEntity.h"
#include "core-Renderer\StaticGeometryRenderable.h"
#include "core-Renderer\RenderableNode.h"
#include "ext-RendererView\SceneManagers.h"


namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

World::World(WorldEntity& entity)
: m_entity(entity)
{
}

///////////////////////////////////////////////////////////////////////////////

World::~World()
{
}

///////////////////////////////////////////////////////////////////////////////

void World::initialize(RendererView& rendererView)
{
   int maxElemsPerLeaf = 64;
   int maxTreeDepth = 5;
   StaticSceneManager* staticGeometry = new StaticSceneManager(m_entity.m_size, 
      maxElemsPerLeaf, 
      maxTreeDepth);

   DynamicSceneManager* dynamicGeometry = new DynamicSceneManager(new LinearStorage<RenderableNode>());

   rendererView.setStaticSceneManager(staticGeometry);
   rendererView.setDynamicSceneManager(dynamicGeometry);
}

///////////////////////////////////////////////////////////////////////////////

void World::deinitialize(RendererView& rendererView)
{
}

///////////////////////////////////////////////////////////////////////////////

void World::update(float timeElapsed)
{
   // world size doesn't change
}

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer

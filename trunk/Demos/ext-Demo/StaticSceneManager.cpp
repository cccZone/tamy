#include "ext-Demo\StaticSceneManager.h"
#include "core-Renderer\StaticGeometryRenderable.h"


namespace demo
{

///////////////////////////////////////////////////////////////////////////////

StaticSceneManager::StaticSceneManager(const AABoundingBox& treeBB, 
                                       int maxElements, 
                                       int maxDepth,
                                       int initDepth)
: m_staticSceneTree(NULL)
{
   m_staticSceneTree = new StaticSceneTree(treeBB, maxElements, maxDepth, *this);
}

///////////////////////////////////////////////////////////////////////////////

StaticSceneManager::~StaticSceneManager()
{
   delete m_staticSceneTree;
   m_staticSceneTree = NULL;

   for (RenderablesSet::iterator it = m_renderables.begin();
      it != m_renderables.end(); ++it)
   {
      delete *it;
   }
}

///////////////////////////////////////////////////////////////////////////////

void StaticSceneManager::addObj(StaticGeometryRenderable* elem)
{
   insert(*elem);
   manage(elem);
}

///////////////////////////////////////////////////////////////////////////////

void StaticSceneManager::insert(StaticGeometryRenderable& elem)
{
   m_staticSceneTree->insert(elem);
}

///////////////////////////////////////////////////////////////////////////////

void StaticSceneManager::remove(StaticGeometryRenderable& elem)
{
   m_staticSceneTree->remove(elem);
}

///////////////////////////////////////////////////////////////////////////////

void StaticSceneManager::query(const BoundingVolume& boundingVol, 
                               Array<StaticGeometryRenderable*>& output) const
{
   m_staticSceneTree->query(boundingVol, output);
}

///////////////////////////////////////////////////////////////////////////////

void StaticSceneManager::manage(StaticGeometryRenderable* renderable)
{
   m_renderables.insert(renderable);
}

///////////////////////////////////////////////////////////////////////////////

void StaticSceneManager::unmanage(StaticGeometryRenderable& renderable)
{
   RenderablesSet::iterator it = m_renderables.find(&renderable);
   if (it != m_renderables.end())
   {
      m_renderables.erase(it);
      delete &renderable;
   }
}

///////////////////////////////////////////////////////////////////////////////

} // namespace demo

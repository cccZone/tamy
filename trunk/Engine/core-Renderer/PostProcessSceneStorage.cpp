#include "core-Renderer\PostProcessSceneStorage.h"
#include "core-Renderer\PostProcessEffectRenderable.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

PostProcessSceneStorage::PostProcessSceneStorage(PostProcessEffectRenderable* renderable)
: m_renderable(renderable)
{
   if (m_renderable == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a PostProcessEffectRenderable instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

PostProcessSceneStorage::~PostProcessSceneStorage()
{
   delete m_renderable;
   m_renderable = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessSceneStorage::query(Camera& camera, 
                                    Array<Renderable*>& outRenderables)
{
   outRenderables.push_back(m_renderable);
}

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\RenderingMechanismLeaf.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\RenderablesStorage.h"
#include "core-Renderer\RenderingTargetsPolicy.h"
#include "core-Renderer\RendererImpl.h"
#include "core-Renderer\RenderingAction.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

namespace   // anonymous
{
   class NullRenderingAction : public RenderingAction
   {
   public:
      void onRenderingBegin(Renderable& renderable, RendererImpl& impl) {}
      void onRenderingEnd(Renderable& renderable, RendererImpl& impl) {}
   };

   class NullNodesStorage : public RenderablesStorage
   {
   public:
      void query(Camera& camera, Array<Renderable*>& outRenderables) {}
   };

}  // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

RenderingMechanismLeaf::RenderingMechanismLeaf(RenderingTargetsPolicy* policy, 
                                               RendererImpl* impl)
: m_policy(policy)
, m_nodesStorage(new NullNodesStorage())
, m_renderingAction(new NullRenderingAction())
, m_activeCamera(NULL)
, m_impl(impl)
, m_objectsToRender(4096)
{
   if (m_policy == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a RenderingTargetsPolicy instance");
   }

   if (m_impl == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a RendererImpl instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderingMechanismLeaf::~RenderingMechanismLeaf()
{
   m_activeCamera = NULL;

   delete m_renderingAction;
   m_renderingAction = NULL;

   delete m_nodesStorage;
   m_nodesStorage = NULL;

   delete m_impl;
   m_impl = NULL;

   delete m_policy;
   m_policy = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingMechanismLeaf::setStorage(RenderablesStorage* storage)
{
   if (storage == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a RenderablesStorage instance");
   }

   delete m_nodesStorage;
   m_nodesStorage = storage;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingMechanismLeaf::setRenderingAction(RenderingAction* action)
{
   if (action == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a RenderingAction instance");
   }

   delete m_renderingAction;
   m_renderingAction = action;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingMechanismLeaf::setActiveCamera(Camera& camera)
{
   m_activeCamera = &camera;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingMechanismLeaf::render()
{
   if (m_activeCamera == NULL) {return;}

   // query the storage for rendering data
   m_objectsToRender.clear();
   m_nodesStorage->query(*m_activeCamera, m_objectsToRender);

   unsigned int objectsCount = m_objectsToRender.size();
   if (objectsCount == 0) {return;}

   // start the redering process
   m_impl->setMatrices(m_activeCamera->getViewMtx(), 
                       m_activeCamera->getProjectionMtx());

   for (DWORD i = 0; i < objectsCount; ++i)
   {
      Renderable& object = *(m_objectsToRender[i]);

      m_renderingAction->onRenderingBegin(object, *m_impl);
      m_impl->setObjectMatrices(object.getRenderingMatrices());
      m_impl->render(object, *m_policy);
      m_renderingAction->onRenderingEnd(object, *m_impl);
   }
}

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\Scene3DRM.h"
#include "core-Renderer\RenderingView.h"
#include "core-Renderer\SpatialView.h"
#include "core-Renderer\CameraContext.h"
#include "core-Renderer\Camera.h"
#include "core-MVC\Model.h"
#include "core\AABoundingBox.h"
#include "core\Frustum.h"



///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class PlainCameraContext : public CameraContext
   {
   private:
      Camera& m_camera;
      Frustum m_frustum;

   public:
      PlainCameraContext(Camera& camera) : m_camera(camera) {}

      inline const BoundingVolume& getViewVolume() 
      {
         m_frustum = m_camera.getFrustum();
         return m_frustum;
      }
   };
} // anonymous

///////////////////////////////////////////////////////////////////////////////

Scene3DRM::Scene3DRM(Model& scene, Camera& camera)
: m_scene(scene)
, m_camera(new PlainCameraContext(camera))
{
   AABoundingBox sceneBB(D3DXVECTOR3(-10000, -10000, -10000),
      D3DXVECTOR3(10000, 10000, 10000));

   m_renderingView = new RenderingView();
   // TODO: m_renderingView->setAttributeSorter( transparencySorter );

   m_spatialView = new SpatialView(sceneBB);
   m_scene.attach(*m_renderingView);
   m_scene.attach(*m_spatialView);
}

///////////////////////////////////////////////////////////////////////////////

Scene3DRM::~Scene3DRM()
{
   m_scene.detach(*m_renderingView);
   delete m_renderingView; m_renderingView = NULL;

   m_scene.detach(*m_spatialView);
   delete m_spatialView; m_spatialView = NULL;

   delete m_camera; m_camera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Scene3DRM::render()
{
   m_spatialView->update(*m_camera);
   m_renderingView->update();
}

///////////////////////////////////////////////////////////////////////////////

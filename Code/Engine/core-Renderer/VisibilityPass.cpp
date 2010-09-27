#include "core-Renderer\VisibilityPass.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\SpatialView.h"
#include "core-Renderer\CameraContext.h"
#include "core-Renderer\Camera.h"
#include "core-MVC.h"
#include "core.h"


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

VisibilityPass::VisibilityPass( Camera& camera )
: m_camera( camera )
, m_cameraContext( new PlainCameraContext( camera ) )
{
   AABoundingBox sceneBB(D3DXVECTOR3(-10000, -10000, -10000),
      D3DXVECTOR3(10000, 10000, 10000));

   m_spatialView = new SpatialView( sceneBB );
}

///////////////////////////////////////////////////////////////////////////////

VisibilityPass::~VisibilityPass()
{
   for ( ScenesMap::iterator it = m_renderedScenes.begin();
         it != m_renderedScenes.end(); ++it )
   {
      Model* scene = it->first;
      scene->detach( *m_spatialView );

      ModelComponent< Camera >* cameraComponent = it->second;
      scene->removeComponent( *cameraComponent );
      delete cameraComponent;
   }
   delete m_spatialView; m_spatialView = NULL;

   delete m_cameraContext; m_cameraContext = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void VisibilityPass::addScene( Model& model )
{
   // check if this han't already been added
   bool isAdded = m_renderedScenes.find( &model ) != m_renderedScenes.end();
   if ( isAdded )
   {
      return;
   }

   model.attach( *m_spatialView );

   // set the scene with a camera
   ModelComponent< Camera >* camComp = new ModelComponent< Camera >( m_camera );
   model.addComponent( camComp );

   // memorize the scene
   m_renderedScenes.insert( std::make_pair( &model, camComp ) );
}

///////////////////////////////////////////////////////////////////////////////

void VisibilityPass::removeScene( Model& model )
{
   ScenesMap::iterator it = m_renderedScenes.find( &model );
   bool isAdded = ( it != m_renderedScenes.end() );
   if ( !isAdded )
   {
      return;
   }

   Model* scene = it->first;
   scene->detach( *m_spatialView );

   // remove the camera component
   ModelComponent< Camera >* cameraComponent = it->second;
   scene->removeComponent( *cameraComponent );
   delete cameraComponent;

   m_renderedScenes.erase( it );
}

///////////////////////////////////////////////////////////////////////////////

void VisibilityPass::render()
{
   m_spatialView->update( *m_cameraContext );
}

///////////////////////////////////////////////////////////////////////////////

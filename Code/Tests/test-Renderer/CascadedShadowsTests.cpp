#include "core-TestFramework\TestFramework.h"
#include "core/Math.h"
#include "core-Renderer/CascadedShadowsUtils.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/Renderer.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class RendererImplementationMock : public Renderer
   {
   protected:
      void resetViewport(unsigned int width, unsigned int height) {}
      void resizeViewport( unsigned int width, unsigned int height ) {}
      void renderingBegin() {}
      void renderingEnd() {}
      bool isGraphicsSystemReady() const {return true;}
      void attemptToRecoverGraphicsSystem() {}
      void activateRenderTarget( RenderTarget* renderTarget, uint targetIdx ) {}
      void deactivateRenderTarget( uint targetIdx ) {}
      void cleanRenderTarget( const Color& bgColor ) {}
      void activateDepthBuffer( DepthBuffer& buffer ) {}
   };
} // anonymous


///////////////////////////////////////////////////////////////////////////////

TEST( CascadedShadowsUtils, singleCascadeStage )
{
   CascadesConfig cascadesConfig( 1, 1, 1 );
   cascadesConfig.m_cascadeIntervals[0] = 0;
   cascadesConfig.m_cascadeIntervals[1] = 10.0f;
   cascadesConfig.m_cascadeDimensions = 1024.0f;


   // Light is shining from up above directly down along the negative OY axis.
   // Since a light shines towards its local OZ axis, a transformation that will
   // achieve the desired shining direction must rotate the OZ vector so that it becomes an OY_NEG vector.
   // Pitching the vector ( rotating it about OX axis ) by 90 degrees will do just that.
   Transform lightTransform;
   lightTransform.m_rotation.setAxisAngle( Vector::OX, DEG2RAD( 90.0 ) );
   lightTransform.toMatrix( cascadesConfig.m_lightRotationMtx );

   // let's define the scene boundaries
   cascadesConfig.m_sceneBounds = AABoundingBox( Vector( -100, -100, -100 ), Vector( 100, 100, 100 ) );

   // finally define the camera
   RendererImplementationMock renderer;
   Camera camera( "camera", renderer, Camera::PT_PERSPECTIVE );
   cascadesConfig.m_activeCamera = &camera;

   camera.setNearPlaneDimensions( 1000.0f, 1000.0f );
   camera.setClippingPlanes( 1.0f, 1000.0f );
   camera.setFOV( 60.0f );


   // calculate the light bounds
   CascadeStage         calculatedCascadeStage;
   Transform            cameraTransform;
   Vector               lightPos;

   // position the camera in the center of the scene, looking down the Z axis
   {
      cameraTransform = Transform::IDENTITY;
      cameraTransform.toMatrix( camera.accessLocalMtx() );
      CascadedShadowsUtils::calculateCascadesBounds( cascadesConfig, &calculatedCascadeStage );

      // object query bounds give us a volume that embraces all objects that we want to render using this cascade stage
      COMPARE_VEC( Vector( -5.7904153f, -100, 0.97844368f ), calculatedCascadeStage.m_objectsQueryBounds.min );
      COMPARE_VEC( Vector(  5.7904153f,  100, 10.004807f  ), calculatedCascadeStage.m_objectsQueryBounds.max );

      // the light bounds are expressed in light's view space - if we want to query for scene objects using it, we need to
      // transform them back to world space
      COMPARE_VEC( Vector( -5.7904153f, 0.97844368f,  0 ), calculatedCascadeStage.m_lightFrustumBounds.min );
      COMPARE_VEC( Vector(  5.7904153f, 10.004807f,   200.0f ), calculatedCascadeStage.m_lightFrustumBounds.max );

      // the light stays on top of the scene bounding box, hovering above and in front of the camera position ( in the projected
      // center of the cascade frustum in front of the camera )
      COMPARE_VEC( Vector( 0, 100, 5.4916182f ), calculatedCascadeStage.m_lightMtx.position() );
   }

   // move the camera further away from origin, without rotating it
   {
      cameraTransform.m_translation.set( 10, 20, 30 );
      cameraTransform.toMatrix( camera.accessLocalMtx() );
      CascadedShadowsUtils::calculateCascadesBounds( cascadesConfig, &calculatedCascadeStage );

      // the query bounds move... - we moved the camera, so naturally we'll be rendering objects from a spot right in front of it
      COMPARE_VEC( Vector(  4.2070994f, -100, 30.984070f ), calculatedCascadeStage.m_objectsQueryBounds.min );
      COMPARE_VEC( Vector( 15.787931f,   100, 40.010437f  ), calculatedCascadeStage.m_objectsQueryBounds.max );

      COMPARE_VEC( Vector(  4.2070994f, 30.984070f, 0 ), calculatedCascadeStage.m_lightFrustumBounds.min );
      COMPARE_VEC( Vector( 15.787931f,  40.010437f, 200 ), calculatedCascadeStage.m_lightFrustumBounds.max );

      COMPARE_VEC( Vector( 9.9975157f, 100, 35.497231f ), calculatedCascadeStage.m_lightMtx.position() );
   }

   // now rotate the camera only ( move it back to the origin )
   {
      cameraTransform.m_rotation.setAxisAngle( Vector::OY, DEG2RAD( 90.0f ) );
      cameraTransform.m_translation.set( 0, 0, 0 );
      cameraTransform.toMatrix( camera.accessLocalMtx() );
      CascadedShadowsUtils::calculateCascadesBounds( cascadesConfig, &calculatedCascadeStage );

      COMPARE_VEC( Vector( 0.97844368f, -100, -5.7904153f ), calculatedCascadeStage.m_objectsQueryBounds.min );
      COMPARE_VEC( Vector( 10.004807f,   100,  5.7904153f ), calculatedCascadeStage.m_objectsQueryBounds.max );

      COMPARE_VEC( Vector( 0.97844368f, -5.7904153f, 0 ), calculatedCascadeStage.m_lightFrustumBounds.min );
      COMPARE_VEC( Vector( 10.004807f,   5.7904153f, 200 ), calculatedCascadeStage.m_lightFrustumBounds.max );

      COMPARE_VEC( Vector( 5.4916282f, 100, 0 ), calculatedCascadeStage.m_lightMtx.position() );
   }

   // now rotate and translate the camera
   {
      cameraTransform.m_rotation.setAxisAngle( Vector::OY, DEG2RAD( 90.0f ) );
      cameraTransform.m_translation.set( 10, 20, 30 );
      cameraTransform.toMatrix( camera.accessLocalMtx() );
      CascadedShadowsUtils::calculateCascadesBounds( cascadesConfig, &calculatedCascadeStage );

      COMPARE_VEC( Vector( 10.983256f, -100, 24.202131f ), calculatedCascadeStage.m_objectsQueryBounds.min );
      COMPARE_VEC( Vector( 20.009624f,  100, 35.782978f ), calculatedCascadeStage.m_objectsQueryBounds.max );

      COMPARE_VEC( Vector( 10.983256f, 24.202137f, 0 ), calculatedCascadeStage.m_lightFrustumBounds.min );
      COMPARE_VEC( Vector( 20.009624f, 35.782970f, 200 ), calculatedCascadeStage.m_lightFrustumBounds.max );

      COMPARE_VEC( Vector( 15.496441f, 100, 29.992544f ), calculatedCascadeStage.m_lightMtx.position() );
   }

   // finally - rotate the light source so that it's shining along the Z axis, 
   // but test it for the initial camera position ( in the origin, not translated ).
   // This way we have a light shining in the same direction the camera's looking ( shining at our back so to speak )
   {
      lightTransform.m_rotation = Quaternion::getIdentity();
      lightTransform.toMatrix( cascadesConfig.m_lightRotationMtx );

      cameraTransform = Transform::IDENTITY;
      cameraTransform.toMatrix( camera.accessLocalMtx() );
      CascadedShadowsUtils::calculateCascadesBounds( cascadesConfig, &calculatedCascadeStage );

      COMPARE_VEC( Vector( -5.7904153f, -5.7904153f, -100 ),  calculatedCascadeStage.m_objectsQueryBounds.min );
      COMPARE_VEC( Vector(  5.7904153f,  5.7904153f,  100  ), calculatedCascadeStage.m_objectsQueryBounds.max );

      COMPARE_VEC( Vector( -5.7904153f, -5.7904153f, 0 ), calculatedCascadeStage.m_lightFrustumBounds.min );
      COMPARE_VEC( Vector(  5.7904153f,  5.7904153f, 200 ), calculatedCascadeStage.m_lightFrustumBounds.max );

      COMPARE_VEC( Vector( 0, 0, -100 ), calculatedCascadeStage.m_lightMtx.position() );
   }

   // roll the light source
   {
      lightTransform.m_rotation.setAxisAngle( Vector::OZ, DEG2RAD( 45.0f ) );
      lightTransform.toMatrix( cascadesConfig.m_lightRotationMtx );

      cameraTransform = Transform::IDENTITY;
      cameraTransform.toMatrix( camera.accessLocalMtx() );
      CascadedShadowsUtils::calculateCascadesBounds( cascadesConfig, &calculatedCascadeStage );

      COMPARE_VEC( Vector( -11.580832f, -11.580832f, -100 ),  calculatedCascadeStage.m_objectsQueryBounds.min );
      COMPARE_VEC( Vector(  11.580832f,  11.580832f,  100  ), calculatedCascadeStage.m_objectsQueryBounds.max );

      COMPARE_VEC( Vector( -8.1888838f, -8.1888838f, 0 ), calculatedCascadeStage.m_lightFrustumBounds.min );
      COMPARE_VEC( Vector(  8.1888838f,  8.1888838f, 200 ), calculatedCascadeStage.m_lightFrustumBounds.max );

      COMPARE_VEC( Vector( 0, 0, -100 ), calculatedCascadeStage.m_lightMtx.position() );
   }

}

///////////////////////////////////////////////////////////////////////////////

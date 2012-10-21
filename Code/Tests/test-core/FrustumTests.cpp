#include "core-TestFramework\TestFramework.h"
#include "core\Math.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Camera.h"


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

TEST( Frustum, frustumBoundingBox )
{
   RendererImplementationMock renderer;
   Camera camera( "camera", renderer, Camera::PT_PERSPECTIVE );
   camera.setNearPlaneDimensions(10, 10);
   camera.setClippingPlanes(10, 100);
   camera.setFOV(90);

   Frustum frustum;
   AABoundingBox frustumBounds;
   Transform cameraTransform;

   // identity matrix
   camera.calculateFrustum( frustum );
   frustum.calculateBoundingBox( frustumBounds );
   COMPARE_VEC( Vector( -100.0f, -100.0f, 10.0f ), frustumBounds.min );
   COMPARE_VEC( Vector(  100.0f,  100.0f, 100.0f ), frustumBounds.max );

   // changed position
   cameraTransform.m_translation.set( 10, 20, 30 );
   cameraTransform.toMatrix( camera.accessLocalMtx() );
   camera.calculateFrustum( frustum );
   frustum.calculateBoundingBox( frustumBounds );
   COMPARE_VEC( Vector( -90.0f, -80.0f, 40.0f ), frustumBounds.min );
   COMPARE_VEC( Vector(  110.0f,  120.0f, 130.0f ), frustumBounds.max );

   // rotated & translated camera
   cameraTransform.m_rotation.setAxisAngle( Vector_OY, FastFloat::fromFloat( DEG2RAD( 90 ) ) );
   cameraTransform.toMatrix( camera.accessLocalMtx() );
   camera.calculateFrustum( frustum );
   frustum.calculateBoundingBox( frustumBounds );
   COMPARE_VEC( Vector( 20.0f, -80.0f, -70.0f ), frustumBounds.min );
   COMPARE_VEC( Vector(  110.0f, 120.0f, 130.0f ), frustumBounds.max );

   // rotated camera
   cameraTransform.m_translation.set( 0, 0, 0 );
   cameraTransform.toMatrix( camera.accessLocalMtx() );
   camera.calculateFrustum( frustum );
   frustum.calculateBoundingBox( frustumBounds );
   COMPARE_VEC( Vector( 10.0f, -100.0f, -100.0f ), frustumBounds.min );
   COMPARE_VEC( Vector(  100.0f, 100.0f, 100.0f ), frustumBounds.max );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Frustum, pointFrustumIntersection )
{
   RendererImplementationMock renderer;

   Camera camera( "camera", renderer, Camera::PT_PERSPECTIVE );
   camera.setNearPlaneDimensions(10, 10);
   camera.setClippingPlanes(0.1f, 100);
   camera.setFOV(90);

   Frustum frustum;
   PointVolume ptVolume( Vector_ZERO );
   camera.calculateFrustum( frustum );

   ptVolume.point.set( 0, 0, 1 );
   CPPUNIT_ASSERT_EQUAL( true, frustum.testCollision( ptVolume ) );

   ptVolume.point.set( 0, 0, 100.0f );
   CPPUNIT_ASSERT_EQUAL( true, frustum.testCollision( ptVolume ) );

   ptVolume.point.set( -50, 50, 100.0f  );
   CPPUNIT_ASSERT_EQUAL( true, frustum.testCollision( ptVolume ) );

   ptVolume.point.set( -5, -5, 10.0f  );
   CPPUNIT_ASSERT_EQUAL( true, frustum.testCollision( ptVolume ) );

   ptVolume.point.set( -5, 5, 10.0f  );
   CPPUNIT_ASSERT_EQUAL( true, frustum.testCollision( ptVolume ) );

   ptVolume.point.set( 5, -5, 10.0f  );
   CPPUNIT_ASSERT_EQUAL( true, frustum.testCollision( ptVolume ) );

   ptVolume.point.set( 5, 5, 10.0f  );
   CPPUNIT_ASSERT_EQUAL( true, frustum.testCollision( ptVolume ) );


   ptVolume.point.set( 0, 0, 0 );
   CPPUNIT_ASSERT_EQUAL( false, frustum.testCollision( ptVolume ) );

   ptVolume.point.set( 0, 0, 101.0f );
   CPPUNIT_ASSERT_EQUAL( false, frustum.testCollision( ptVolume ) );

   ptVolume.point.set( -15, -15, 10.0f  );
   CPPUNIT_ASSERT_EQUAL( false, frustum.testCollision( ptVolume ) );

   ptVolume.point.set( -15, 15, 10.0f  );
   CPPUNIT_ASSERT_EQUAL( false, frustum.testCollision( ptVolume ) );

   ptVolume.point.set( 15, -15, 10.0f  );
   CPPUNIT_ASSERT_EQUAL( false, frustum.testCollision( ptVolume ) );

   ptVolume.point.set( 15, 15, 10.0f  );
   CPPUNIT_ASSERT_EQUAL( false, frustum.testCollision( ptVolume ) );
}

///////////////////////////////////////////////////////////////////////////////

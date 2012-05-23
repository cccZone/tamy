#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Camera.h"
#include "core-TestFramework\MatrixWriter.h"
#include "core\Frustum.h"
#include "core\BoundingSphere.h"
#include "core\CollisionTests.h"
#include "core\Ray.h"
#include "core-Renderer\Renderer.h"
#include "core\Math.h"

///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class RendererImplementationMock : public Renderer
   {
   protected:
      void resetViewport(unsigned int width, unsigned int height) {}
      void renderingBegin() {}
      void renderingEnd() {}
      bool isGraphicsSystemReady() const {return true;}
      void attemptToRecoverGraphicsSystem() {}
      void activateRenderTarget( RenderTarget* renderTarget ) {}
      void cleanRenderTarget( const Color& bgColor ) {}
   };
} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(Camera, frustrumCreation)
{
   RendererImplementationMock renderer;
   Camera camera( "camera", renderer, Camera::PT_PERSPECTIVE );
   camera.setNearPlaneDimensions(10, 10);
   camera.setClippingPlanes(10, 100);
   camera.setFOV(90);

   Frustum frustrum = camera.getFrustum();

   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(Vector(0, 0, -2), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(Vector(0, 0, 8), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(Vector(0, 0, 102), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(Vector(0, 0, 0), 1)));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 0, 100), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 0, 8), 2)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 0, 100), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 0, 50), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(-20, 0, 50), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(20, 0, 50), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, -20, 50), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 20, 50), 1)));

   // camera rotated
   Quaternion rotY;
   rotY.setAxisAngle( Vector::OY, DEG2RAD( 90 ) );
   camera.accessLocalMtx().setRotation( rotY );

   frustrum = camera.getFrustum();

   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(10, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(100, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(50, 0, 0), 1)));

   // camera rotated
   rotY.setAxisAngle( Vector::OY, DEG2RAD( -90 ) );
   camera.accessLocalMtx().setRotation( rotY );

   frustrum = camera.getFrustum();

   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(-10, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(-100, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(-50, 0, 0), 1)));

}

///////////////////////////////////////////////////////////////////////////////

TEST(Camera, createRay)
{
   RendererImplementationMock renderer;
   Camera camera( "camera", renderer, Camera::PT_PERSPECTIVE);
   Frustum frustum = camera.getFrustum();
   Vector expectedNormal;

   // a basic plane running straight through the screen's center
   Ray result;
   camera.createRay( 0, 0, result );
   COMPARE_VEC( Vector(0, 0, 0), result.origin );
   COMPARE_VEC( Vector(0, 0, 1), result.direction );

   // test the rays running through the frustum's clipping planes:
   // - bottom plane
   Vector planeNormal(frustum.planes[FP_BOTTOM].a, frustum.planes[FP_BOTTOM].b, frustum.planes[FP_BOTTOM].c);
   expectedNormal.setCross( planeNormal, Vector::OX ).normalize();

   camera.createRay( 0, -1, result );
   COMPARE_VEC(Vector(0, 0, 0), result.origin);
   COMPARE_VEC(expectedNormal, result.direction);

   // - top plane
   planeNormal = Vector(frustum.planes[FP_TOP].a, frustum.planes[FP_TOP].b, frustum.planes[FP_TOP].c);
   expectedNormal.setCross( planeNormal, Vector::OX_NEG ).normalize();

   camera.createRay( 0, 1, result );
   COMPARE_VEC(Vector(0, 0, 0), result.origin);
   COMPARE_VEC(expectedNormal, result.direction);

   // - left plane
   planeNormal = Vector(frustum.planes[FP_LEFT].a, frustum.planes[FP_LEFT].b, frustum.planes[FP_LEFT].c);
   expectedNormal.setCross( Vector::OY, planeNormal ).normalize();

   camera.createRay( -1, 0, result );
   COMPARE_VEC(Vector(0, 0, 0), result.origin);
   COMPARE_VEC(expectedNormal, result.direction);

   // - right plane
   planeNormal = Vector(frustum.planes[FP_RIGHT].a, frustum.planes[FP_RIGHT].b, frustum.planes[FP_RIGHT].c);
   expectedNormal.setCross( Vector::OY_NEG, planeNormal ).normalize();

   camera.createRay( 1, 0, result );
   COMPARE_VEC(Vector(0, 0, 0), result.origin);
   COMPARE_VEC(expectedNormal, result.direction);
}

///////////////////////////////////////////////////////////////////////////////

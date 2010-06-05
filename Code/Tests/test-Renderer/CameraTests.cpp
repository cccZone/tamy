#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Camera.h"
#include "core\MatrixWriter.h"
#include "core\Frustum.h"
#include "core\BoundingSphere.h"
#include "core\CollisionTests.h"
#include "core\Ray.h"
#include "core-Renderer\Renderer.h"


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
   Camera camera("camera", renderer);
   camera.setNearPlaneDimensions(10, 10);
   camera.setClippingPlanes(10, 100);
   camera.setFOV(90);

   Frustum frustrum = camera.getFrustum();

   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, -2), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 8), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 102), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 0), 1)));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 100), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 8), 2)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 100), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 50), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(-20, 0, 50), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(20, 0, 50), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, -20, 50), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 20, 50), 1)));

   // camera rotated
   D3DXMatrixRotationY(&(camera.accessLocalMtx()), D3DXToRadian(90));

   frustrum = camera.getFrustum();

   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(10, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(100, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(50, 0, 0), 1)));

   // camera rotated
   D3DXMatrixRotationY(&(camera.accessLocalMtx()), D3DXToRadian(-90));

   frustrum = camera.getFrustum();

   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(-10, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(-100, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(-50, 0, 0), 1)));

}

///////////////////////////////////////////////////////////////////////////////

TEST(Camera, createRay)
{
   RendererImplementationMock renderer;
   Camera camera("camera", renderer);
   Frustum frustum = camera.getFrustum();
   D3DXVECTOR3 expectedNormal;
   D3DXVECTOR3 nox(-1, 0, 0);
   D3DXVECTOR3 ox(1, 0, 0);
   D3DXVECTOR3 noy(0, -1, 0);
   D3DXVECTOR3 oy(0, 1, 0);

   // a basic plane running straight through the screen's center
   Ray result = camera.createRay(0, 0);
   COMPARE_VEC(D3DXVECTOR3(0, 0, 0), result.origin);
   COMPARE_VEC(D3DXVECTOR3(0, 0, 1), result.direction);

   // test the rays running through the frustum's clipping planes:
   // - bottom plane
   D3DXVECTOR3 planeNormal(frustum.planes[FP_BOTTOM].a, frustum.planes[FP_BOTTOM].b, frustum.planes[FP_BOTTOM].c);
   D3DXVec3Cross(&expectedNormal, &planeNormal, &ox);
   D3DXVec3Normalize(&expectedNormal, &expectedNormal);

   result = camera.createRay(0, -1);
   COMPARE_VEC(D3DXVECTOR3(0, 0, 0), result.origin);
   COMPARE_VEC(expectedNormal, result.direction);

   // - top plane
   planeNormal = D3DXVECTOR3(frustum.planes[FP_TOP].a, frustum.planes[FP_TOP].b, frustum.planes[FP_TOP].c);
   D3DXVec3Cross(&expectedNormal, &planeNormal, &nox);
   D3DXVec3Normalize(&expectedNormal, &expectedNormal);

   result = camera.createRay(0, 1);
   COMPARE_VEC(D3DXVECTOR3(0, 0, 0), result.origin);
   COMPARE_VEC(expectedNormal, result.direction);

   // - left plane
   planeNormal = D3DXVECTOR3(frustum.planes[FP_LEFT].a, frustum.planes[FP_LEFT].b, frustum.planes[FP_LEFT].c);
   D3DXVec3Cross(&expectedNormal, &oy, &planeNormal);
   D3DXVec3Normalize(&expectedNormal, &expectedNormal);

   result = camera.createRay(-1, 0);
   COMPARE_VEC(D3DXVECTOR3(0, 0, 0), result.origin);
   COMPARE_VEC(expectedNormal, result.direction);

   // - right plane
   planeNormal = D3DXVECTOR3(frustum.planes[FP_RIGHT].a, frustum.planes[FP_RIGHT].b, frustum.planes[FP_RIGHT].c);
   D3DXVec3Cross(&expectedNormal, &noy, &planeNormal);
   D3DXVec3Normalize(&expectedNormal, &expectedNormal);

   result = camera.createRay(1, 0);
   COMPARE_VEC(D3DXVECTOR3(0, 0, 0), result.origin);
   COMPARE_VEC(expectedNormal, result.direction);
}

///////////////////////////////////////////////////////////////////////////////

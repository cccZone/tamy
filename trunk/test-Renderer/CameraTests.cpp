#include "core-TestFramework\TestFramework.h"
#include "SceneRenderingMechanismMock.h"
#include "RendererImplementationMock.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\Camera.h"
#include "core\MatrixWriter.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core\Frustum.h"
#include "core\BoundingSphere.h"
#include "core\CollisionTests.h"
#include "core\Ray.h"
#include "RenderingTechniqueStub.h"
#include "RendererImplementationMock.h"


using namespace RegularTests;

///////////////////////////////////////////////////////////////////////////////

TEST(Camera, renderingWithActiveCamera)
{
   RendererImplementationMock renderer;

   SceneRenderingMechanismMock renderingMechanism;
   Camera camera1("camera1", renderer);
   Camera camera2("camera2", renderer);

   D3DXMATRIX camera1Mtx = camera1.getLocalMtx();
   camera1Mtx._41 = 5;
   camera1.setLocalMtx(camera1Mtx);

   D3DXMATRIX camera2Mtx = camera2.getLocalMtx();
   camera2Mtx._41 = 10;
   camera2.setLocalMtx(camera2Mtx);

   RenderingTechniqueStub technique;
   std::vector<RenderingTechnique*> techniques; 
   techniques.push_back(&technique);
   GraphicalEntityMock entity("", techniques);
   GraphicalNode* node = new GraphicalNode("", false, entity, 0);

   VisualSceneManager sceneManager;
   renderingMechanism.addVisualSceneManager(sceneManager);
   sceneManager.setActiveCamera(camera1);
   sceneManager.addNode(node);

   renderingMechanism.render();     

   D3DXMatrixInverse(&camera1Mtx, NULL, &camera1Mtx);
   D3DXMATRIX viewMtx = renderingMechanism.getViewMatrixSet();
   for (int col = 0; col < 4; col++)
   {
      for (int row = 0; row < 4; row++)
      {
         CPPUNIT_ASSERT_DOUBLES_EQUAL(camera1Mtx.m[col][row], viewMtx.m[col][row], 0.01);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
/*
TEST(Camera, settingAspectRatioOfCurrentViewportDuringRendering)
{
RendererImplementationMock renderer;
RendererImplementationMock renderingMechanism;
Camera camera("camera", renderer);

VisualSceneManager sceneManager;
renderingMechanism.addVisualSceneManager(sceneManager);
sceneManager.setActiveCamera(camera);

// we resized the viewport
renderingMechanism.resizeViewport(100, 50, 0, 0, 800, 600);

// camera's not attached yet - there's the default aspect ratio there
CPPUNIT_ASSERT_DOUBLES_EQUAL(1.333f, camera.getAspectRatio(), 0.01);

// no we set the camera as an active one
renderingMechanism.render();
CPPUNIT_ASSERT_DOUBLES_EQUAL(2, camera.getAspectRatio(), 0.01);
}
// TODO: PRZYWROCIC !!!!!!
*/
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

#include "core-TestFramework\TestFramework.h"
#include "RendererImplementationMock.h"
#include "core-Renderer\Material.h"
#include "TextureStub.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\Camera.h"
#include "core\MatrixWriter.h"
#include "VisualSceneManagerMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Camera, renderingWithActiveCamera)
{
   RendererImplementationMock renderer;
   Camera camera1("camera1");
   Camera camera2("camera2");

   D3DXMATRIX camera1Mtx = camera1.getLocalMtx();
   camera1Mtx._41 = 5;
   camera1.setLocalMtx(camera1Mtx);

   D3DXMATRIX camera2Mtx = camera2.getLocalMtx();
   camera2Mtx._41 = 10;
   camera2.setLocalMtx(camera2Mtx);
  
   TextureStub tex("");
   Material mat(tex);
   std::vector<Material*> materials; materials.push_back(&mat);
   GraphicalEntityMock entity("", materials);
   GraphicalNode* node = new GraphicalNode("", entity, 0);

   VisualSceneManagerMock sceneManager;
   renderer.addVisualSceneManager(sceneManager);
   sceneManager.setActiveCamera(camera1);
   sceneManager.addNode(node);

   renderer.render();     

   D3DXMatrixInverse(&camera1Mtx, NULL, &camera1Mtx);
   D3DXMATRIX viewMtx = renderer.getViewMatrixSet();
   for (int col = 0; col < 4; col++)
   {
      for (int row = 0; row < 4; row++)
      {
         CPPUNIT_ASSERT_DOUBLES_EQUAL(camera1Mtx.m[col][row], viewMtx.m[col][row], 0.01);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST(Camera, settingAspectRatioOfCurrentViewportDuringRendering)
{
   RendererImplementationMock renderer;
   Camera camera("camera");

   VisualSceneManagerMock sceneManager;
   renderer.addVisualSceneManager(sceneManager);
   sceneManager.setActiveCamera(camera);

   // we resized the viewport
   renderer.resizeViewport(100, 50);

   // camera's not attached yet - there's the default aspect ratio there
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.333f, camera.getAspectRatio(), 0.01);

   // no we set the camera as an active one
   renderer.render();
   CPPUNIT_ASSERT_DOUBLES_EQUAL(2, camera.getAspectRatio(), 0.01);
}

///////////////////////////////////////////////////////////////////////////////

#include "core-TestFramework\TestFramework.h"
#include "VisualSceneManagerMock.h"
#include "core-Renderer\Camera.h"
#include "SkyBoxMock.h"
#include "core\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, addingCameraAndActiveCamera)
{
   VisualSceneManagerMock sceneManager;
   Camera* camera1 = new Camera("camera1");
   Camera* camera2 = new Camera("camera2");

   CPPUNIT_ASSERT_EQUAL(false, sceneManager.hasActiveCamera());

   sceneManager.addNode(camera1);
   CPPUNIT_ASSERT_EQUAL(true, sceneManager.hasActiveCamera());
   CPPUNIT_ASSERT_EQUAL(std::string("camera1"), sceneManager.getActiveCamera().getName());

   sceneManager.addNode(camera2);
   CPPUNIT_ASSERT_EQUAL(true, sceneManager.hasActiveCamera());
   CPPUNIT_ASSERT_EQUAL(std::string("camera1"), sceneManager.getActiveCamera().getName());
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, removingCameraThatIsSetAsActive)
{
   VisualSceneManagerMock sceneManager;
   Camera camera("camera");

   sceneManager.addNode(&camera);
   CPPUNIT_ASSERT_EQUAL(true, sceneManager.hasActiveCamera());
  
   sceneManager.removeNode(camera);
   CPPUNIT_ASSERT_EQUAL(false, sceneManager.hasActiveCamera());
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, changingActiveCameraReattachesSkyBox)
{
   VisualSceneManagerMock sceneManager;
   Camera* camera1 = new Camera("camera1");
   Camera* camera2 = new Camera("camera2");
   sceneManager.addNode(camera1);
   sceneManager.addNode(camera2);
   SkyBoxMock* skyBox = new SkyBoxMock();
   sceneManager.setSkyBox(skyBox);
   D3DXMatrixTranslation(&(camera2->accessLocalMtx()), 20, 0, 0);

   D3DXMATRIX expectedResult;

   sceneManager.setActiveCamera(*camera1);
   D3DXMatrixIdentity(&expectedResult);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox->getGlobalMtx());

   sceneManager.setActiveCamera(*camera2);
   D3DXMatrixIdentity(&expectedResult);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox->getLocalMtx());
   D3DXMatrixTranslation(&expectedResult, 20, 0, 0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox->getGlobalMtx());
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, addingSkyBoxAttachesItToActiveCamera)
{
   VisualSceneManagerMock sceneManager;
   Camera* camera = new Camera("camera");
   sceneManager.addNode(camera);

   SkyBoxMock* skyBox1 = new SkyBoxMock();
   SkyBoxMock* skyBox2 = new SkyBoxMock();

   D3DXMATRIX expectedResult;

   sceneManager.setSkyBox(skyBox1);
   D3DXMatrixIdentity(&expectedResult);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox1->getGlobalMtx());
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox2->getGlobalMtx());

   D3DXMatrixTranslation(&(camera->accessLocalMtx()), 20, 0, 0);
   D3DXMatrixTranslation(&expectedResult, 20, 0, 0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox1->getGlobalMtx());
   D3DXMatrixIdentity(&expectedResult);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox2->getGlobalMtx());

   sceneManager.setSkyBox(skyBox2);
   D3DXMatrixTranslation(&expectedResult, 20, 0, 0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox2->getGlobalMtx());

   D3DXMatrixTranslation(&(camera->accessLocalMtx()), 20, 40, 0);
   D3DXMatrixTranslation(&expectedResult, 20, 40, 0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox2->getGlobalMtx());
}

///////////////////////////////////////////////////////////////////////////////

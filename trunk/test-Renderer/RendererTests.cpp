#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\RenderingCommand.h"
#include "core-Renderer\BasicSceneManager.h"
#include "RendererImplementationMock.h"
#include "TextureStub.h"
#include "core-Renderer\Material.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "SkyBoxMock.h"
#include "LightReflectingPropertiesStub.h"
#include "SceneManagerMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, renderingOnlyWhenRenderingDeviceIsReady)
{
   SceneManagerMock sceneManager;
   RendererImplementationMock renderer;

   renderer.render(sceneManager); 
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasPresentCalled());

   renderer.setDeviceReady(false);

   renderer.render(sceneManager);
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasPresentCalled());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, recoveringTheLostDevice)
{
   SceneManagerMock sceneManager;
   RendererImplementationMock renderer;

   // initial rendering
   renderer.render(sceneManager); 
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade());

   // we loose the device
   renderer.setDeviceReady(false);
   renderer.render(sceneManager); 
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade()); // we don't attempt to recover the device just yet - let's give it aloop

   renderer.render(sceneManager); 
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasDeviceRecoveryAttemptMade());

   // the RendererImplementationMock will consider every recovery attempt a success, so the next
   // rendering pass should proceed as if the device was ready

   renderer.render(sceneManager); 
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade());

   renderer.render(sceneManager); 
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, renderingSkyBox)
{
   SceneManagerMock sceneManager;
   RendererImplementationMock renderer;

   SkyBoxMock* skyBox = new SkyBoxMock();
   sceneManager.setSkyBox(skyBox);

   renderer.render(sceneManager); 

   CPPUNIT_ASSERT(skyBox->wasRendered());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, renderingTransparentObjectsInSecondPass)
{
   SceneManagerMock sceneManager;

   RendererImplementationMock renderer;
   renderer.render(sceneManager);

   CPPUNIT_ASSERT_EQUAL(0, sceneManager.getRegularNodesQuerySequence());
   CPPUNIT_ASSERT_EQUAL(1, sceneManager.getTransparentNodesQuerySequence());
}

///////////////////////////////////////////////////////////////////////////////

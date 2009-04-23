#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\RenderingCommand.h"
#include "core-Renderer\BasicVisualSceneManager.h"
#include "RendererImplementationMock.h"
#include "TextureStub.h"
#include "core-Renderer\Material.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "LightReflectingPropertiesStub.h"
#include "VisualSceneManagerMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, renderingOnlyWhenRenderingDeviceIsReady)
{
   VisualSceneManagerMock sceneManager;
   RendererImplementationMock renderer;
   renderer.addVisualSceneManager(sceneManager);


   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasPresentCalled());

   renderer.setDeviceReady(false);

   renderer.render();
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasPresentCalled());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, recoveringTheLostDevice)
{
   VisualSceneManagerMock sceneManager;
   RendererImplementationMock renderer;
   renderer.addVisualSceneManager(sceneManager);

   // initial rendering
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade());

   // we loose the device
   renderer.setDeviceReady(false);
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade()); // we don't attempt to recover the device just yet - let's give it aloop

   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasDeviceRecoveryAttemptMade());

   // the RendererImplementationMock will consider every recovery attempt a success, so the next
   // rendering pass should proceed as if the device was ready

   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade());

   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade());
}

///////////////////////////////////////////////////////////////////////////////

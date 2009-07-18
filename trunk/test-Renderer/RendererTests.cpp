#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\VisualSceneManager.h"
#include "RendererImplementationMock.h"
#include "core-Renderer\VisualSceneManager.h"
#include "RenderingTargetMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, renderingOnlyWhenRenderingDeviceIsReady)
{
   VisualSceneManager sceneManager;
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
   VisualSceneManager sceneManager;
   RendererImplementationMock renderer;
   renderer.addVisualSceneManager(sceneManager);

   // initial rendering
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade());

   // we loose the device
   renderer.setDeviceReady(false);
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade()); // we don't attempt to recover the device just yet - let's give it a loop

   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasDeviceRecoveryAttemptMade());

   // the RendererImplementationMock will consider every recovery attempt a success, so the next
   // rendering pass should proceed as if the device was ready

   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade());

   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, noRenderingWithoutRenderingTargets)
{
   VisualSceneManager sceneManager;
   RendererImplementationMock renderer(false);
   renderer.addVisualSceneManager(sceneManager);

   renderer.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, renderer.getRenderingTargetsCount());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasPresentCalled());

   RenderingTargetMock target;

   renderer.addRenderingTarget(target);
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, renderer.getRenderingTargetsCount());
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasPresentCalled());
   CPPUNIT_ASSERT_EQUAL(0, target.getRenderingIdx());

   renderer.removeRenderingTarget(target);
   renderer.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, renderer.getRenderingTargetsCount());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasPresentCalled());
   CPPUNIT_ASSERT_EQUAL(-1, target.getRenderingIdx());
}

///////////////////////////////////////////////////////////////////////////////

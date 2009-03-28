#include "core-TestFramework\TestFramework.h"
#include "RendererImplementationMock.h"
#include "LightMock.h"
#include "core\Node.h"
#include "core-Renderer\BasicSceneManager.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Lights, singleLight)
{
   RendererImplementationMock renderer; 
   LightMock* light = new LightMock();

   BasicSceneManager sceneManager;
   sceneManager.addNode(light);

   renderer.render(sceneManager);
   CPPUNIT_ASSERT_EQUAL(true, light->hasBeenEnabled()); // lights gete nabled during the rendering pass
   CPPUNIT_ASSERT_EQUAL(false, light->isEnabled()); // lights get turned off after rendering pass is complete
}

///////////////////////////////////////////////////////////////////////////////

TEST(Lights, renderingHierarchyOfLights)
{
   RendererImplementationMock renderer; 

   Node* root = new Node();
   LightMock* light1 = new LightMock();
   LightMock* light2 = new LightMock();
   root->addChild(light1);
   root->addChild(light2);

   BasicSceneManager sceneManager;
   sceneManager.addNode(root);

   renderer.render(sceneManager);

   // only as many lights can be set as the device light limit allows
   CPPUNIT_ASSERT_EQUAL(true, light1->hasBeenEnabled());
   CPPUNIT_ASSERT_EQUAL(true, light2->hasBeenEnabled());

   // lights get turned off after rendering pass is complete
   CPPUNIT_ASSERT_EQUAL(false, light1->isEnabled());
   CPPUNIT_ASSERT_EQUAL(false, light2->isEnabled());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Lights, tooManyLights)
{
   RendererImplementationMock renderer; 
   renderer.setMaxLightsCount(1); // we can only render using ONE LIGHT at a time !!!!

   Node* root = new Node();
   LightMock* light1 = new LightMock();
   LightMock* light2 = new LightMock();
   root->addChild(light1);
   root->addChild(light2);

   BasicSceneManager sceneManager;
   sceneManager.addNode(root);

   renderer.render(sceneManager);

   // only as many lights can be set as the device light limit allows
   CPPUNIT_ASSERT_EQUAL(true, light1->hasBeenEnabled());
   CPPUNIT_ASSERT_EQUAL(false, light2->hasBeenEnabled());

   // lights get turned off after rendering pass is complete
   CPPUNIT_ASSERT_EQUAL(false, light1->isEnabled());
   CPPUNIT_ASSERT_EQUAL(false, light2->isEnabled());
}

///////////////////////////////////////////////////////////////////////////////

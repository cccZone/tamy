#include "core-TestFramework\TestFramework.h"
#include "SceneRenderingMechanismMock.h"
#include "RendererImplementationMock.h"
#include "LightMock.h"
#include "core\Node.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\Camera.h"
#include "RendererImplementationMock.h"


using namespace RegularTests;

///////////////////////////////////////////////////////////////////////////////

TEST(Lights, singleLight)
{
   RendererImplementationMock renderer;
   Camera camera("camera", renderer);
   SceneRenderingMechanismMock renderingMechanism; 
   LightMock* light = new LightMock();

   VisualSceneManager sceneManager;
   renderingMechanism.addVisualSceneManager(sceneManager);
   sceneManager.setActiveCamera(camera);
   sceneManager.addNode(light);

   renderingMechanism.render();
   CPPUNIT_ASSERT_EQUAL(true, light->hasBeenEnabled()); // lights get enabled during the rendering pass
   CPPUNIT_ASSERT_EQUAL(false, light->isEnabled()); // lights get turned off after rendering pass is complete
}

///////////////////////////////////////////////////////////////////////////////

TEST(Lights, renderingHierarchyOfLights)
{
   RendererImplementationMock renderer;
   Camera camera("camera", renderer);
   SceneRenderingMechanismMock renderingMechanism; 

   Node* root = new Node("node", false);
   LightMock* light1 = new LightMock();
   LightMock* light2 = new LightMock();
   root->addChild(light1);
   root->addChild(light2);

   VisualSceneManager sceneManager;
   renderingMechanism.addVisualSceneManager(sceneManager);
   sceneManager.setActiveCamera(camera);
   sceneManager.addNode(root);
      
   renderingMechanism.render();

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
   Camera camera("camera", renderer);
   SceneRenderingMechanismMock renderingMechanism(1); // we can only render using ONE LIGHT at a time !!!!

   Node* root = new Node("root", false);
   LightMock* light1 = new LightMock();
   LightMock* light2 = new LightMock();
   root->addChild(light1);
   root->addChild(light2);

   VisualSceneManager sceneManager;
   renderingMechanism.addVisualSceneManager(sceneManager);
   sceneManager.setActiveCamera(camera);
   sceneManager.addNode(root);

   renderingMechanism.render();

   // only as many lights can be set as the device light limit allows
   CPPUNIT_ASSERT_EQUAL(true, light1->hasBeenEnabled());
   CPPUNIT_ASSERT_EQUAL(false, light2->hasBeenEnabled());

   // lights get turned off after rendering pass is complete
   CPPUNIT_ASSERT_EQUAL(false, light1->isEnabled());
   CPPUNIT_ASSERT_EQUAL(false, light2->isEnabled());
}

///////////////////////////////////////////////////////////////////////////////

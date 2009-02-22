#include "TestFramework.h"
#include "RendererImplementationMock.h"
#include "LightMock.h"
#include "Node.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Lights, singleLight)
{
   RendererImplementationMock renderer; 
   LightMock light;

   renderer.render(light);
   CPPUNIT_ASSERT_EQUAL(true, light.hasBeenEnabled()); // lights gete nabled during the rendering pass
   CPPUNIT_ASSERT_EQUAL(false, light.isEnabled()); // lights get turned off after rendering pass is complete
}

///////////////////////////////////////////////////////////////////////////////

TEST(Lights, renderingHierarchyOfLights)
{
   RendererImplementationMock renderer; 

   Node root;
   LightMock* light1 = new LightMock();
   LightMock* light2 = new LightMock();
   root.addChild(light1);
   root.addChild(light2);

   renderer.render(root);
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

   Node root;
   LightMock* light1 = new LightMock();
   LightMock* light2 = new LightMock();
   root.addChild(light1);
   root.addChild(light2);

   renderer.render(root);
   // only as many lights can be set as the device light limit allows
   CPPUNIT_ASSERT_EQUAL(true, light1->hasBeenEnabled());
   CPPUNIT_ASSERT_EQUAL(false, light2->hasBeenEnabled());

   // lights get turned off after rendering pass is complete
   CPPUNIT_ASSERT_EQUAL(false, light1->isEnabled());
   CPPUNIT_ASSERT_EQUAL(false, light2->isEnabled());
}

///////////////////////////////////////////////////////////////////////////////

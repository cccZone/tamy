#include "TestFramework.h"
#include "RenderingProcessor.h"
#include "RenderingCommand.h"
#include "GraphicalNodesAggregator.h"
#include "RendererImplementationMock.h"
#include "TextureStub.h"
#include "Material.h"
#include "GraphicalEntityMock.h"
#include "GraphicalNode.h"
#include "SkyBoxMock.h"
#include "LightReflectingPropertiesStub.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, renderingOnlyWhenRenderingDeviceIsReady)
{
   RendererImplementationMock renderer;

   TextureStub tex("");
   Material mat(tex);
   std::vector<Material*> materials; materials.push_back(&mat);
   GraphicalEntityMock entity(materials);
   GraphicalNode node("", entity, 0);

   renderer.render(node);
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasPresentCalled());

   renderer.setDeviceReady(false);

   renderer.render(node);
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasPresentCalled());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, recoveringTheLostDevice)
{
   RendererImplementationMock renderer;
   TextureStub tex("");
   Material mat(tex);
   std::vector<Material*> materials; materials.push_back(&mat);
   GraphicalEntityMock entity(materials);
   GraphicalNode node("", entity, 0);

   // initial rendering
   renderer.render(node);
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade());

   // we loose the device
   renderer.setDeviceReady(false);
   renderer.render(node);
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade()); // we don't attempt to recover the device just yet - let's give it aloop

   renderer.render(node);
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasDeviceRecoveryAttemptMade());

   // the RendererImplementationMock will consider every recovery attempt a success, so the next
   // rendering pass should proceed as if the device was ready

   renderer.render(node);
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(true, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade());

   renderer.render(node);
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasInitialStateSet());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasViewportReset());
   CPPUNIT_ASSERT_EQUAL(false, renderer.wasDeviceRecoveryAttemptMade());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, renderingSkyBox)
{
   RendererImplementationMock renderer;
   SkyBoxMock skyBox;
   renderer.setSkyBox(skyBox);

   TextureStub tex("");
   Material mat(tex);
   std::vector<Material*> materials; materials.push_back(&mat);
   GraphicalEntityMock entity(materials);
   GraphicalNode node("", entity, 0);
   renderer.render(node);

   CPPUNIT_ASSERT(skyBox.wasRendered());
}

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingProcessor, issuingRenderingCommands)
{
   RenderingProcessor processor;
   std::list<std::string> results;

   // prepare the material
   TextureStub texture(results);
   Material material(texture);
   LightReflectingPropertiesStub lrp(results);
   material.setLightReflectingProperties(lrp);

   // create the node we'll use for rendering
   std::vector<Material*> materials; materials.push_back(&material);
   GraphicalEntityMock entity("entity", materials, results);
   GraphicalNode node("", entity, 0);

   // run the processor
   std::list<GraphicalNode*> nodesToRender;
   nodesToRender.push_back(&node);
   std::list<RenderingCommand> commands = processor.translate(nodesToRender);
   
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (2), commands.size());

   // run through the comands
   for (std::list<RenderingCommand>::iterator it = commands.begin(); it != commands.end(); ++it)
   {
      RenderingCommand& comm = *it;
      comm();
   }

   // compare the results
   std::vector<std::string> expectedResults;
   expectedResults.push_back("Set light reflecting properties");
   expectedResults.push_back("Set texture");
   expectedResults.push_back("Render entity subset 0");

   CPPUNIT_ASSERT_EQUAL(expectedResults.size(), results.size());

   unsigned int resultIdx = 0;
   for (std::list<std::string>::iterator resultIt = results.begin(); 
        resultIt != results.end(); ++resultIt, ++resultIdx)
   {
      std::stringstream msgId;
      msgId << "command " << resultIdx;
      CPPUNIT_ASSERT_EQUAL_MESSAGE(msgId.str(), expectedResults.at(resultIdx), *resultIt);
   }
}

///////////////////////////////////////////////////////////////////////////////

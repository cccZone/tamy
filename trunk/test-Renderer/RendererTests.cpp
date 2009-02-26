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
   BatchedNodes nodesToRender;
   nodesToRender.insert(&node);
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
   expectedResults.push_back("Set light reflecting properties 0");
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

TEST(GraphicalNodesAggregator, sortingByMaterialsWhenGatheringNodesForRendering)
{
   // prepare the materials
   TextureStub texture("");

   Material material1(texture, 0);
   Material material2(texture, 1);

   // create the node we'll use for rendering
   std::vector<Material*> materials; 
   materials.push_back(&material1);
   materials.push_back(&material1);
   materials.push_back(&material2);
   materials.push_back(&material1);
   GraphicalEntityMock entity("entity", materials, D3DMATRIX());

   Node root;
   GraphicalNode* node1 = new GraphicalNode("subset0 - material1", entity, 0);
   GraphicalNode* node2 = new GraphicalNode("subset1 - material1", entity, 1);
   GraphicalNode* node3 = new GraphicalNode("subset2 - material2", entity, 2);
   GraphicalNode* node4 = new GraphicalNode("subset3 - material1", entity, 3);
   root.addChild(node1);
   root.addChild(node2);
   root.addChild(node3);
   root.addChild(node4);

   GraphicalNodesAggregator aggregator;
   root.accept(aggregator);

   std::vector<std::string> expResults;
   expResults.push_back("subset0 - material1");
   expResults.push_back("subset1 - material1");
   expResults.push_back("subset3 - material1");
   expResults.push_back("subset2 - material2");
   unsigned int resultIdx = 0;
   for (BatchedNodes::const_iterator resultIt = aggregator().begin();
        resultIt != aggregator().end(); ++resultIt, ++resultIdx)
   {
      std::stringstream msgId;
      msgId << "command " << resultIdx;
      CPPUNIT_ASSERT_EQUAL_MESSAGE(msgId.str(), expResults.at(resultIdx), (*resultIt)->getName());
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingProcessor, materialNotSetIfItDoesntChange)
{
   RenderingProcessor processor;
   std::list<std::string> results;

   // prepare the materials
   TextureStub texture(results);
   LightReflectingPropertiesStub lrp1(results, 0);
   LightReflectingPropertiesStub lrp2(results, 1);

   Material material1(texture, 0);
   material1.setLightReflectingProperties(lrp1);
   Material material2(texture, 1);
   material2.setLightReflectingProperties(lrp2);

   // create the node we'll use for rendering
   std::vector<Material*> materials; 
   materials.push_back(&material1);
   materials.push_back(&material1);
   materials.push_back(&material2);
   materials.push_back(&material1);
   GraphicalEntityMock entity("entity", materials, results);
   GraphicalNode node1("", entity, 0);
   GraphicalNode node2("", entity, 1);
   GraphicalNode node3("", entity, 2);
   GraphicalNode node4("", entity, 3);

   // run the processor
   BatchedNodes nodesToRender;
   nodesToRender.insert(&node1);
   nodesToRender.insert(&node2);
   nodesToRender.insert(&node3);
   nodesToRender.insert(&node4);
   std::list<RenderingCommand> commands = processor.translate(nodesToRender);
   
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (6), commands.size());

   // run through the comands
   for (std::list<RenderingCommand>::iterator it = commands.begin(); it != commands.end(); ++it)
   {
      RenderingCommand& comm = *it;
      comm();
   }

   // compare the results
   std::vector<std::string> expectedResults;
   expectedResults.push_back("Set light reflecting properties 0");
   expectedResults.push_back("Set texture");
   expectedResults.push_back("Render entity subset 0");
   expectedResults.push_back("Render entity subset 1");
   expectedResults.push_back("Render entity subset 3");
   expectedResults.push_back("Set light reflecting properties 1");
   expectedResults.push_back("Set texture");
   expectedResults.push_back("Render entity subset 2");

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

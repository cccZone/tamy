#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\RenderingCommand.h"
#include "TextureStub.h"
#include "core-Renderer\Material.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "LightReflectingPropertiesStub.h"


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
   DWORD nodesArraySize = 1;
   AbstractGraphicalNodeP* nodesToRender = new AbstractGraphicalNodeP[nodesArraySize];
   nodesToRender[0] = &node;

   DWORD commandsArraySize = 10;
   RenderingCommand* commands = new RenderingCommand[commandsArraySize];

   DWORD commandsCount = processor.translate(nodesToRender, nodesArraySize, 
                                             commands, commandsArraySize);
   
   CPPUNIT_ASSERT_EQUAL((DWORD)2, commandsCount);

   // run through the comands
   for (DWORD i = 0; i < commandsCount; ++i)
   {
      (commands[i])();
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

   delete [] nodesToRender;
   delete [] commands;
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
   DWORD nodesArraySize = 4;
   AbstractGraphicalNodeP* nodesToRender = new AbstractGraphicalNodeP[nodesArraySize];
   nodesToRender[0] = &node1;
   nodesToRender[1] = &node2;
   nodesToRender[2] = &node4;
   nodesToRender[3] = &node3; // this one uses a different material

   DWORD commandsArraySize = 10;
   RenderingCommand* commands = new RenderingCommand[commandsArraySize];

   DWORD commandsCount = processor.translate(nodesToRender, nodesArraySize, 
                                             commands, commandsArraySize);

   
   CPPUNIT_ASSERT_EQUAL((DWORD)6, commandsCount);

   // run through the comands
   for (DWORD i = 0; i < commandsCount; ++i)
   {
      (commands[i])();
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

   delete [] nodesToRender;
   delete [] commands;
}

///////////////////////////////////////////////////////////////////////////////

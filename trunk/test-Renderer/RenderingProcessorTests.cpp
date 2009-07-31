#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\RenderingProcessor.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\Material.h"
#include "MaterialImplMock.h"
#include "RenderingTargetsPolicyMock.h"


using namespace RenderingProcessorTests;

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingProcessor, materialNotSetIfItDoesntChange)
{
   RenderingProcessor processor;
   std::vector<std::string> results;

   // create the node we'll use for rendering
   Material material1("material1"); material1.getLightReflectingProperties().setPower(2);
   Material material2("material2"); material2.getLightReflectingProperties().setPower(3);
   std::vector<Material*> materials; 
   materials.push_back(&material1);
   materials.push_back(&material1);
   materials.push_back(&material2); // this one uses a different material
   materials.push_back(&material1);
   GraphicalEntityMock entity("entity", materials, results);
   GraphicalNode node1("", false, entity, 0);
   GraphicalNode node2("", false, entity, 1);
   GraphicalNode node3("", false, entity, 2);
   GraphicalNode node4("", false, entity, 3);

   // run the processor
   Array<AbstractGraphicalNode*> nodesToRender;
   nodesToRender.push_back(&node1);
   nodesToRender.push_back(&node2);
   nodesToRender.push_back(&node3); // the one with the different material is set here
   nodesToRender.push_back(&node4);

   MaterialImplMock materialImpl(results);
   RenderingProcessorTests::RenderingTargetsPolicyMock policy(results);
   processor.translate(nodesToRender, materialImpl, policy);

   // compare the results
   std::vector<std::string> expectedResults;
   expectedResults.push_back("Setting rendering targets");
   expectedResults.push_back("Set material with power = 2.0");
   expectedResults.push_back("Render entity subset 0");
   expectedResults.push_back("Render entity subset 1");
   expectedResults.push_back("Set material with power = 3.0");
   expectedResults.push_back("Render entity subset 2");
   expectedResults.push_back("Set material with power = 2.0");
   expectedResults.push_back("Render entity subset 3");

   CPPUNIT_ASSERT_EQUAL(expectedResults.size(), results.size());

   unsigned int resultIdx = 0;
   for (std::vector<std::string>::iterator resultIt = results.begin(); 
        resultIt != results.end(); ++resultIt, ++resultIdx)
   {
      std::stringstream msgId;
      msgId << "command " << resultIdx;
      CPPUNIT_ASSERT_EQUAL_MESSAGE(msgId.str(), expectedResults.at(resultIdx), *resultIt);
   }
}

///////////////////////////////////////////////////////////////////////////////

#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\RenderingProcessor.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "RenderingTechniqueStub.h"


///////////////////////////////////////////////////////////////////////////////

TEST(RenderingProcessor, materialNotSetIfItDoesntChange)
{
   RenderingProcessor processor;
   std::vector<std::string> results;

   // create the node we'll use for rendering
   RenderingProcessorTests::RenderingTechniqueStub technique1("technique1", results);
   RenderingProcessorTests::RenderingTechniqueStub technique2("technique2",results);
   std::vector<RenderingTechnique*> techniques; 
   techniques.push_back(&technique1);
   techniques.push_back(&technique1);
   techniques.push_back(&technique2);
   techniques.push_back(&technique1);
   GraphicalEntityMock entity("entity", techniques, results);
   GraphicalNode node1("", false, entity, 0);
   GraphicalNode node2("", false, entity, 1);
   GraphicalNode node3("", false, entity, 2);
   GraphicalNode node4("", false, entity, 3);

   // run the processor
   Array<AbstractGraphicalNode*> nodesToRender;
   nodesToRender.push_back(&node1);
   nodesToRender.push_back(&node2);
   nodesToRender.push_back(&node4);
   nodesToRender.push_back(&node3); // this one uses a different material

   processor.translate(nodesToRender);

   // compare the results
   std::vector<std::string> expectedResults;
   expectedResults.push_back("Set technique technique1");
   expectedResults.push_back("Render entity subset 0");
   expectedResults.push_back("Render entity subset 1");
   expectedResults.push_back("Render entity subset 3");
   expectedResults.push_back("Set technique technique2");
   expectedResults.push_back("Render entity subset 2");

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

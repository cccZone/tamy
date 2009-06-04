#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\RenderingCommand.h"
#include "TextureStub.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "MaterialOperationImplementationMock.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "LightReflectingPropertiesStub.h"


///////////////////////////////////////////////////////////////////////////////

TEST(RenderingProcessor, issuingRenderingCommands)
{
   RenderingProcessor processor;
   std::list<std::string> results;
   MaterialOperationImplementationMock matOpImpl;

   // prepare the material
   TextureStub texture(results);
   LightReflectingPropertiesStub lrp(results);

   Material material(lrp, matOpImpl, matOpImpl);
   MaterialStage* materialStage = new MaterialStage(texture,  
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));
   material.addStage(materialStage);

   // create the node we'll use for rendering
   std::vector<Material*> materials; materials.push_back(&material);
   GraphicalEntityMock entity("entity", materials, results);
   GraphicalNode node("", false, entity, 0);

   // run the processor
   DWORD nodesArraySize = 1;
   AbstractGraphicalNodeP* nodesToRender = new AbstractGraphicalNodeP[nodesArraySize];
   nodesToRender[0] = &node;

   processor.translate(nodesToRender, nodesArraySize);
   

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
}

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingProcessor, materialNotSetIfItDoesntChange)
{
   RenderingProcessor processor;
   std::list<std::string> results;
   MaterialOperationImplementationMock matOpImpl;

   // prepare the materials
   TextureStub texture(results);
   LightReflectingPropertiesStub lrp1(results, 0);
   LightReflectingPropertiesStub lrp2(results, 1);

   Material material1(lrp1, matOpImpl, matOpImpl, 0);
   MaterialStage* materialStage1 = new MaterialStage(texture, 
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));
   material1.addStage(materialStage1);

   Material material2(lrp2, matOpImpl, matOpImpl, 1);
   MaterialStage* materialStage2 = new MaterialStage(texture,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));
   material2.addStage(materialStage2);

   // create the node we'll use for rendering
   std::vector<Material*> materials; 
   materials.push_back(&material1);
   materials.push_back(&material1);
   materials.push_back(&material2);
   materials.push_back(&material1);
   GraphicalEntityMock entity("entity", materials, results);
   GraphicalNode node1("", false, entity, 0);
   GraphicalNode node2("", false, entity, 1);
   GraphicalNode node3("", false, entity, 2);
   GraphicalNode node4("", false, entity, 3);

   // run the processor
   DWORD nodesArraySize = 4;
   AbstractGraphicalNodeP* nodesToRender = new AbstractGraphicalNodeP[nodesArraySize];
   nodesToRender[0] = &node1;
   nodesToRender[1] = &node2;
   nodesToRender[2] = &node4;
   nodesToRender[3] = &node3; // this one uses a different material

   processor.translate(nodesToRender, nodesArraySize);

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
}

///////////////////////////////////////////////////////////////////////////////

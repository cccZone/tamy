#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\RenderingTechniqueReplacer.h"
#include "AbstractGraphicalNodeMock.h"
#include "RenderingTechniqueStub.h"


using namespace RegularTests;

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingTechniqueReplacer, replacingSelectedTechnique)
{

   RenderingTechniqueStub technique1("technique1");
   RenderingTechniqueStub technique2("technique2");

   AbstractGraphicalNodeMock* graphNode1 = new AbstractGraphicalNodeMock(technique1);
   AbstractGraphicalNodeMock* graphNode2 = new AbstractGraphicalNodeMock(technique2);
   AbstractGraphicalNodeMock* graphNode3 = new AbstractGraphicalNodeMock(technique2);

   Node* compositeNode = new Node("compositeNode", false);
   compositeNode->addChild(graphNode1);
   compositeNode->addChild(graphNode2);

   Node rootNode("rootNode", true);
   rootNode.addChild(compositeNode);
   rootNode.addChild(graphNode3);
   
   // run the tests
   RenderingTechniqueReplacer replacer(technique2, technique1);
   rootNode.accept(replacer);

   CPPUNIT_ASSERT_EQUAL(technique1.getName(), graphNode1->getTechnique().getName());
   CPPUNIT_ASSERT_EQUAL(technique1.getName(), graphNode2->getTechnique().getName());
   CPPUNIT_ASSERT_EQUAL(technique1.getName(), graphNode3->getTechnique().getName());
}

///////////////////////////////////////////////////////////////////////////////

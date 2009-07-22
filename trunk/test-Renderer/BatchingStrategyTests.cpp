#include "core-TestFramework\TestFramework.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\BatchingStrategy.h"
#include "RenderingTechniqueStub.h"


using namespace RegularTests;

///////////////////////////////////////////////////////////////////////////////

TEST(BatchComparator, sortingByMaterials)
{
   // create the node we'll use for rendering
   RenderingTechniqueStub technique1;
   RenderingTechniqueStub technique2;
   std::vector<RenderingTechnique*> techniques; 
   techniques.push_back(&technique1);
   techniques.push_back(&technique1);
   techniques.push_back(&technique2);
   techniques.push_back(&technique1);
   GraphicalEntityMock entity("entity", techniques);

   GraphicalNode node1("subset0 - material1", false, entity, 0);
   GraphicalNode node2("subset1 - material1", false, entity, 1);
   GraphicalNode node3("subset2 - material2", false, entity, 2);
   GraphicalNode node4("subset3 - material1", false, entity, 3);

   BatchComparator comparator;
   CPPUNIT_ASSERT_EQUAL(false, comparator(&node1, &node2));
   CPPUNIT_ASSERT_EQUAL(false, comparator(&node2, &node1));

   CPPUNIT_ASSERT_EQUAL(true, comparator(&node2, &node3));
   CPPUNIT_ASSERT_EQUAL(false, comparator(&node3, &node2));

   CPPUNIT_ASSERT_EQUAL(false, comparator(&node2, &node4));
   CPPUNIT_ASSERT_EQUAL(false, comparator(&node4, &node2));

}

///////////////////////////////////////////////////////////////////////////////

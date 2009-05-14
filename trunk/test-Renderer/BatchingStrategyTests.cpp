#include "core-TestFramework\TestFramework.h"
#include "TextureStub.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "MaterialOperationImplementationMock.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "LightReflectingPropertiesStub.h"
#include "core-Renderer\BatchingStrategy.h"


///////////////////////////////////////////////////////////////////////////////

TEST(BatchComparator, sortingByMaterials)
{
   // prepare the materials
   TextureStub texture("");
   MaterialOperationImplementationMock matOpImpl;
   LightReflectingPropertiesStub lrp;

   Material material1(lrp, 0);
   Material material2(lrp, 1);
   material1.addStage(new MaterialStage(texture,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));
   material2.addStage(new MaterialStage(texture,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));

   // create the node we'll use for rendering
   std::vector<Material*> materials; 
   materials.push_back(&material1);
   materials.push_back(&material1);
   materials.push_back(&material2);
   materials.push_back(&material1);
   GraphicalEntityMock entity("entity", materials);

   GraphicalNode node1("subset0 - material1", entity, 0);
   GraphicalNode node2("subset1 - material1", entity, 1);
   GraphicalNode node3("subset2 - material2", entity, 2);
   GraphicalNode node4("subset3 - material1", entity, 3);

   BatchComparator comparator;
   CPPUNIT_ASSERT_EQUAL(false, comparator(&node1, &node2));
   CPPUNIT_ASSERT_EQUAL(false, comparator(&node2, &node1));

   CPPUNIT_ASSERT_EQUAL(true, comparator(&node2, &node3));
   CPPUNIT_ASSERT_EQUAL(false, comparator(&node3, &node2));

   CPPUNIT_ASSERT_EQUAL(false, comparator(&node2, &node4));
   CPPUNIT_ASSERT_EQUAL(false, comparator(&node4, &node2));

}

///////////////////////////////////////////////////////////////////////////////

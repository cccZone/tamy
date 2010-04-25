#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\ManagedTexture.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include "GraphicalNodeMock.h"



///////////////////////////////////////////////////////////////////////////////

template<>
class TTextureImplDestructor<int>
{
public:
   void destroy(int* tex) {}
};

///////////////////////////////////////////////////////////////////////////////

TEST(MaterialStage, equalityWithTwoEqualLightReflectanceProperties)
{
   ManagedTexture emptyTex("", new TTextureImpl<int>());
   MaterialStage mat1(emptyTex,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_WRAP));
   MaterialStage mat2(emptyTex,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_WRAP));

   CPPUNIT_ASSERT(mat1 == mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(MaterialStage, equalityWithTwoDifferentTextures)
{
   ManagedTexture tex1("tex1", new TTextureImpl<int>());
   ManagedTexture tex2("tex2", new TTextureImpl<int>());
   MaterialStage mat1(tex1,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_WRAP));
   MaterialStage mat2(tex2,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_WRAP));

   CPPUNIT_ASSERT(mat1 != mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(MaterialStage, equalityWithTwoIdenticalTextures)
{
   ManagedTexture tex("tex", new TTextureImpl<int>());
   MaterialStage mat1(tex,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_WRAP));
   MaterialStage mat2(tex,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_WRAP));

   CPPUNIT_ASSERT(mat1 == mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(MaterialStage, equalityWithTwoDifferentCoordinateOperations)
{
   ManagedTexture tex("tex", new TTextureImpl<int>());
   MaterialStage mat1(tex,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_WRAP));
   MaterialStage mat2(tex,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_CLAMP));

   CPPUNIT_ASSERT(mat1 != mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Material, addingStage)
{
   ManagedTexture tex("tex", new TTextureImpl<int>());
   Material mat("");
   MaterialStage* stage1 = new MaterialStage(tex,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_WRAP));
   MaterialStage* stage2 = new MaterialStage(tex,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_WRAP));

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, mat.getStagesCount());
   CPPUNIT_ASSERT_EQUAL((int)-1, stage1->getIndex());
   CPPUNIT_ASSERT_EQUAL((int)-1, stage2->getIndex());

   mat.addStage(stage1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, mat.getStagesCount());
   CPPUNIT_ASSERT_EQUAL((int)0, stage1->getIndex());
   CPPUNIT_ASSERT_EQUAL((int)-1, stage2->getIndex());

   mat.addStage(stage2);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, mat.getStagesCount());
   CPPUNIT_ASSERT_EQUAL((int)0, stage1->getIndex());
   CPPUNIT_ASSERT_EQUAL((int)1, stage2->getIndex());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Material, removingStage)
{
   ManagedTexture tex("tex", new TTextureImpl<int>());

   Material mat("");
   MaterialStage* stage1 = new MaterialStage(tex,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_WRAP));
   MaterialStage* stage2 = new MaterialStage(tex,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_WRAP));
   MaterialStage* stage3 = new MaterialStage(tex,
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperation(CC_WRAP));

   mat.addStage(stage1);
   mat.addStage(stage2);
   mat.addStage(stage3);

   CPPUNIT_ASSERT_EQUAL((unsigned int)3, mat.getStagesCount());
   CPPUNIT_ASSERT_EQUAL((int)0, stage1->getIndex());
   CPPUNIT_ASSERT_EQUAL((int)1, stage2->getIndex());
   CPPUNIT_ASSERT_EQUAL((int)2, stage3->getIndex());

   mat.removeStage(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, mat.getStagesCount());
   CPPUNIT_ASSERT_EQUAL((int)0, stage1->getIndex());
   CPPUNIT_ASSERT_EQUAL((int)1, stage3->getIndex());

   mat.removeStage(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, mat.getStagesCount());
   CPPUNIT_ASSERT_EQUAL((int)0, stage3->getIndex());

   mat.removeStage(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, mat.getStagesCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Material, equality)
{
   Material mat1("xxx");
   Material mat2("xxx");

   CPPUNIT_ASSERT(mat1 != mat2);
   CPPUNIT_ASSERT(mat2 == mat2);
   CPPUNIT_ASSERT(mat1 == mat1);
}

///////////////////////////////////////////////////////////////////////////////

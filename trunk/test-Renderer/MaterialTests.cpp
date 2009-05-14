#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "MaterialOperationImplementationMock.h"
#include "LightReflectingPropertiesStub.h"
#include "TextureStub.h"


///////////////////////////////////////////////////////////////////////////////

TEST(MaterialStage, equalityWithTwoEqualLightReflectanceProperties)
{
   MaterialOperationImplementationMock matOpImpl;
   TextureStub emptyTex("");
   LightReflectingPropertiesStub lrp;
   MaterialStage mat1(emptyTex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));
   MaterialStage mat2(emptyTex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));

   CPPUNIT_ASSERT(mat1 == mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(MaterialStage, equalityWithTwoDifferentTextures)
{
   MaterialOperationImplementationMock matOpImpl;
   LightReflectingPropertiesStub lrp;
   TextureStub tex1("tex1");
   TextureStub tex2("tex2");
   MaterialStage mat1(tex1,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));
   MaterialStage mat2(tex2,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));

   CPPUNIT_ASSERT(mat1 != mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(MaterialStage, equalityWithTwoIdenticalTextures)
{
   MaterialOperationImplementationMock matOpImpl;
   LightReflectingPropertiesStub lrp;
   TextureStub tex("tex");
   MaterialStage mat1(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));
   MaterialStage mat2(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));

   CPPUNIT_ASSERT(mat1 == mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Material, addingStage)
{
   MaterialOperationImplementationMock matOpImpl;
   LightReflectingPropertiesStub lrp;
   TextureStub tex("tex");
   Material mat(lrp);
   MaterialStage* stage1 = new MaterialStage(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));
   MaterialStage* stage2 = new MaterialStage(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));

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
   MaterialOperationImplementationMock matOpImpl;
   LightReflectingPropertiesStub lrp;
   TextureStub tex("tex");
   Material mat(lrp);
   MaterialStage* stage1 = new MaterialStage(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));
   MaterialStage* stage2 = new MaterialStage(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));
   MaterialStage* stage3 = new MaterialStage(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));

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

TEST(Material, equalityWithManyStages)
{
   MaterialOperationImplementationMock matOpImpl;
   LightReflectingPropertiesStub lrp;
   TextureStub tex1("tex1");
   TextureStub tex2("tex2");

   Material mat1(lrp);
   mat1.addStage(new MaterialStage(tex1,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));

   Material mat2(lrp);
   mat2.addStage(new MaterialStage(tex1,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));
   mat2.addStage(new MaterialStage(tex2,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));

   Material mat3(lrp);
   mat3.addStage(new MaterialStage(tex2,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));
   mat3.addStage(new MaterialStage(tex1,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));

   Material mat4(lrp);
   mat4.addStage(new MaterialStage(tex1,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));
   mat4.addStage(new MaterialStage(tex2,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));

   CPPUNIT_ASSERT(mat1 != mat2);
   CPPUNIT_ASSERT(mat2 != mat3);
   CPPUNIT_ASSERT(mat3 != mat4);
   CPPUNIT_ASSERT(mat1 != mat3);
   CPPUNIT_ASSERT(mat4 == mat2);
}

///////////////////////////////////////////////////////////////////////////////

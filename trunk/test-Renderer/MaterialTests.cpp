#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "MaterialOperationImplementationMock.h"
#include "LightReflectingPropertiesStub.h"
#include "TextureStub.h"
#include "TransparencyEnablerStub.h"


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
   TextureStub tex("tex");
   TransparencyEnablerStub transparencyEnabler;

   Material mat("", new LightReflectingPropertiesStub(), matOpImpl, matOpImpl, transparencyEnabler);
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
   TextureStub tex("tex");
   TransparencyEnablerStub transparencyEnabler;

   Material mat("", new LightReflectingPropertiesStub(), matOpImpl, matOpImpl, transparencyEnabler);
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

TEST(Material, equality)
{
   MaterialOperationImplementationMock matOpImpl;
   TransparencyEnablerStub transparencyEnabler;

   Material mat1("xxx", new LightReflectingPropertiesStub(), matOpImpl, matOpImpl, transparencyEnabler);
   Material mat2("xxx", new LightReflectingPropertiesStub(), matOpImpl, matOpImpl, transparencyEnabler);

   CPPUNIT_ASSERT(mat1 != mat2);
   CPPUNIT_ASSERT(mat2 == mat2);
   CPPUNIT_ASSERT(mat1 == mat1);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Material, lastRenderedStageIsOneThatDisablesItself)
{
   MaterialOperationImplementationMock colorMatOpImpl;
   MaterialOperationImplementationMock alphaMatOpImpl;
   TextureStub tex("tex");
   TransparencyEnablerStub transparencyEnabler;

   Material mat("", new LightReflectingPropertiesStub(), alphaMatOpImpl, colorMatOpImpl, transparencyEnabler);

   mat.setForRendering();
   CPPUNIT_ASSERT_EQUAL(1, colorMatOpImpl.getOperationsCount());
   CPPUNIT_ASSERT_EQUAL(1, alphaMatOpImpl.getOperationsCount());
   CPPUNIT_ASSERT_EQUAL(std::string("disable"), colorMatOpImpl.getOperation(0));
   CPPUNIT_ASSERT_EQUAL(std::string("disable"), alphaMatOpImpl.getOperation(0));

   colorMatOpImpl.clear();
   alphaMatOpImpl.clear();
   mat.addStage(new MaterialStage(tex,
         new MaterialOperation(colorMatOpImpl, MOP_MULTIPLY, SC_NONE, SC_NONE),
         new MaterialOperation(alphaMatOpImpl, MOP_ADD,      SC_NONE, SC_NONE)));
   mat.setForRendering();
   CPPUNIT_ASSERT_EQUAL(2, colorMatOpImpl.getOperationsCount());
   CPPUNIT_ASSERT_EQUAL(2, alphaMatOpImpl.getOperationsCount());
   CPPUNIT_ASSERT_EQUAL(std::string("multiply"), colorMatOpImpl.getOperation(0));
   CPPUNIT_ASSERT_EQUAL(std::string("add"), alphaMatOpImpl.getOperation(0));
   CPPUNIT_ASSERT_EQUAL(std::string("disable"), colorMatOpImpl.getOperation(1));
   CPPUNIT_ASSERT_EQUAL(std::string("disable"), alphaMatOpImpl.getOperation(1));

   colorMatOpImpl.clear();
   alphaMatOpImpl.clear();
   mat.addStage(new MaterialStage(tex,
         new MaterialOperation(colorMatOpImpl, MOP_SUBTRACT, SC_NONE, SC_NONE),
         new MaterialOperation(alphaMatOpImpl, MOP_MULTIPLY_ADD,      SC_NONE, SC_NONE)));
   mat.setForRendering();
   CPPUNIT_ASSERT_EQUAL(3, colorMatOpImpl.getOperationsCount());
   CPPUNIT_ASSERT_EQUAL(3, alphaMatOpImpl.getOperationsCount());
   CPPUNIT_ASSERT_EQUAL(std::string("multiply"), colorMatOpImpl.getOperation(0));
   CPPUNIT_ASSERT_EQUAL(std::string("add"), alphaMatOpImpl.getOperation(0));
   CPPUNIT_ASSERT_EQUAL(std::string("subtract"), colorMatOpImpl.getOperation(1));
   CPPUNIT_ASSERT_EQUAL(std::string("multiplyAdd"), alphaMatOpImpl.getOperation(1));
   CPPUNIT_ASSERT_EQUAL(std::string("disable"), colorMatOpImpl.getOperation(2));
   CPPUNIT_ASSERT_EQUAL(std::string("disable"), alphaMatOpImpl.getOperation(2));

}

///////////////////////////////////////////////////////////////////////////////

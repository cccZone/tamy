#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\ManagedTexture.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "MaterialOperationImplementationMock.h"
#include "LightReflectingPropertiesStub.h"
#include "TransparencyEnablerStub.h"
#include "CoordinatesOperationMock.h"
#include "GraphicalNodeMock.h"
#include "StageTextureRendererStub.h"
#include "RenderingTargetsPolicyMock.h"


using namespace RegularTests;

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
   StageTextureRendererStub textureRenderer;
   MaterialOperationImplementationMock matOpImpl;
   ManagedTexture emptyTex("", new TTextureImpl<int>());
   LightReflectingPropertiesStub lrp;
   MaterialStage mat1(emptyTex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer);
   MaterialStage mat2(emptyTex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer);

   CPPUNIT_ASSERT(mat1 == mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(MaterialStage, equalityWithTwoDifferentTextures)
{
   StageTextureRendererStub textureRenderer;
   MaterialOperationImplementationMock matOpImpl;
   LightReflectingPropertiesStub lrp;
   ManagedTexture tex1("tex1", new TTextureImpl<int>());
   ManagedTexture tex2("tex2", new TTextureImpl<int>());
   MaterialStage mat1(tex1,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer);
   MaterialStage mat2(tex2,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer);

   CPPUNIT_ASSERT(mat1 != mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(MaterialStage, equalityWithTwoIdenticalTextures)
{
   StageTextureRendererStub textureRenderer;
   MaterialOperationImplementationMock matOpImpl;
   LightReflectingPropertiesStub lrp;
   ManagedTexture tex("tex", new TTextureImpl<int>());
   MaterialStage mat1(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer);
   MaterialStage mat2(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer);

   CPPUNIT_ASSERT(mat1 == mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(MaterialStage, equalityWithTwoDifferentCoordinateOperations)
{
   StageTextureRendererStub textureRenderer;
   MaterialOperationImplementationMock matOpImpl;
   LightReflectingPropertiesStub lrp;
   ManagedTexture tex("tex", new TTextureImpl<int>());
   MaterialStage mat1(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer);
   MaterialStage mat2(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_CLAMP),
         textureRenderer);

   CPPUNIT_ASSERT(mat1 != mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Material, addingStage)
{
   RenderingTargetsPolicyMock renderTargetsPolicy;
   StageTextureRendererStub textureRenderer;
   MaterialOperationImplementationMock matOpImpl;
   ManagedTexture tex("tex", new TTextureImpl<int>());
   TransparencyEnablerStub transparencyEnabler;

   Material mat("", renderTargetsPolicy, new LightReflectingPropertiesStub(), matOpImpl, matOpImpl, transparencyEnabler);
   MaterialStage* stage1 = new MaterialStage(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer);
   MaterialStage* stage2 = new MaterialStage(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer);

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
   RenderingTargetsPolicyMock renderTargetsPolicy;
   StageTextureRendererStub textureRenderer;
   MaterialOperationImplementationMock matOpImpl;
   ManagedTexture tex("tex", new TTextureImpl<int>());
   TransparencyEnablerStub transparencyEnabler;

   Material mat("", renderTargetsPolicy, new LightReflectingPropertiesStub(), matOpImpl, matOpImpl, transparencyEnabler);
   MaterialStage* stage1 = new MaterialStage(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer);
   MaterialStage* stage2 = new MaterialStage(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer);
   MaterialStage* stage3 = new MaterialStage(tex,
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer);

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
   RenderingTargetsPolicyMock renderTargetsPolicy;
   MaterialOperationImplementationMock matOpImpl;
   TransparencyEnablerStub transparencyEnabler;

   Material mat1("xxx", renderTargetsPolicy, new LightReflectingPropertiesStub(), matOpImpl, matOpImpl, transparencyEnabler);
   Material mat2("xxx", renderTargetsPolicy, new LightReflectingPropertiesStub(), matOpImpl, matOpImpl, transparencyEnabler);

   CPPUNIT_ASSERT(mat1 != mat2);
   CPPUNIT_ASSERT(mat2 == mat2);
   CPPUNIT_ASSERT(mat1 == mat1);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Material, lastRenderedStageIsOneThatDisablesItself)
{
   RenderingTargetsPolicyMock renderTargetsPolicy;
   StageTextureRendererStub textureRenderer;
   MaterialOperationImplementationMock colorMatOpImpl;
   MaterialOperationImplementationMock alphaMatOpImpl;
   ManagedTexture tex("tex", new TTextureImpl<int>());
   TransparencyEnablerStub transparencyEnabler;

   Material mat("", renderTargetsPolicy, new LightReflectingPropertiesStub(), alphaMatOpImpl, colorMatOpImpl, transparencyEnabler);

   GraphicalNodeMock node;
   Array<AbstractGraphicalNode*> nodesToRender;
   nodesToRender.push_back(&node);

   mat.render(nodesToRender, 0, 1);
   CPPUNIT_ASSERT_EQUAL(1, colorMatOpImpl.getOperationsCount());
   CPPUNIT_ASSERT_EQUAL(1, alphaMatOpImpl.getOperationsCount());
   CPPUNIT_ASSERT_EQUAL(std::string("disable"), colorMatOpImpl.getOperation(0));
   CPPUNIT_ASSERT_EQUAL(std::string("disable"), alphaMatOpImpl.getOperation(0));

   colorMatOpImpl.clear();
   alphaMatOpImpl.clear();
   mat.addStage(new MaterialStage(tex,
         new MaterialOperation(colorMatOpImpl, MOP_MULTIPLY, SC_NONE, SC_NONE),
         new MaterialOperation(alphaMatOpImpl, MOP_ADD,      SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer));
   mat.render(nodesToRender, 0, 1);
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
         new MaterialOperation(alphaMatOpImpl, MOP_MULTIPLY_ADD,      SC_NONE, SC_NONE),
         new CoordinatesOperationMock(CC_WRAP),
         textureRenderer));
   mat.render(nodesToRender, 0, 1);
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

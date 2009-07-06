#include "core-TestFramework\TestFramework.h"
#include "ext-MaterialsParser\MaterialsParser.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\Texture.h"
#include "MaterialsFactoryMock.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


//////////////////////////////////////////////////////////////////////////////

TEST(MaterialsParser, simpleMaterial)
{
   ResourceStorage<Material> matStorage;
   MaterialsFactoryMock factory;
   MaterialsParser parser(factory, matStorage);

   CPPUNIT_ASSERT_EQUAL(false, matStorage.is("SomeMaterial"));

   parser.load("..\\Data\\SimpleMaterial.xml");
   CPPUNIT_ASSERT_EQUAL(true, matStorage.is("SomeMaterial"));
}

//////////////////////////////////////////////////////////////////////////////

TEST(MaterialsParser, manyMaterialsInOneFile)
{
   ResourceStorage<Material> matStorage;
   MaterialsFactoryMock factory;
   MaterialsParser parser(factory, matStorage);

   CPPUNIT_ASSERT_EQUAL(false, matStorage.is("material_1"));
   CPPUNIT_ASSERT_EQUAL(false, matStorage.is("material_2"));

   parser.load("..\\Data\\ManyMaterials.xml");
   CPPUNIT_ASSERT_EQUAL(true, matStorage.is("material_1"));
   CPPUNIT_ASSERT_EQUAL(true, matStorage.is("material_2"));
}

//////////////////////////////////////////////////////////////////////////////

TEST(MaterialsParser, materialWithNoName)
{
   ResourceStorage<Material> matStorage;
   MaterialsFactoryMock factory;
   MaterialsParser parser(factory, matStorage);

   CPPUNIT_ASSERT_EQUAL(false, matStorage.is(""));

   CPPUNIT_ASSERT_THROW(parser.load("..\\Data\\UnnamedMaterial.xml"), std::runtime_error);

   CPPUNIT_ASSERT_EQUAL(false, matStorage.is(""));
}

//////////////////////////////////////////////////////////////////////////////

TEST(MaterialsParser, defaultLightReflectingProperties)
{
   ResourceStorage<Material> matStorage;
   MaterialsFactoryMock factory;
   MaterialsParser parser(factory, matStorage);

   parser.load("..\\Data\\SimpleMaterial.xml");

   Material& mat = matStorage.get("SomeMaterial");
   const LightReflectingProperties& lrp = mat.getLightReflectingProperties();

   LightReflectingProperties* defaultLRP  = factory.createLightReflectingProperties();
   defaultLRP->setAmbientColor(Color());
   defaultLRP->setDiffuseColor(Color(1, 1, 1, 1));
   defaultLRP->setSpecularColor(Color(1, 1, 1, 1));
   defaultLRP->setEmissiveColor(Color());
   defaultLRP->setPower(1);

   CPPUNIT_ASSERT(lrp == *defaultLRP);

   delete defaultLRP;
}

//////////////////////////////////////////////////////////////////////////////

TEST(MaterialsParser, readingLightReflectingProperties)
{
   ResourceStorage<Material> matStorage;
   MaterialsFactoryMock factory;
   MaterialsParser parser(factory, matStorage);

   parser.load("..\\Data\\MatWithLRP.xml");

   Material& mat = matStorage.get("MatWithLRP");
   const LightReflectingProperties& lrp = mat.getLightReflectingProperties();

   LightReflectingProperties* defaultLRP  = factory.createLightReflectingProperties();
   defaultLRP->setAmbientColor (Color(1.1f, 1.2f, 1.3f, 1.4f));
   defaultLRP->setDiffuseColor (Color(2.1f, 2.2f, 2.3f, 2.4f));
   defaultLRP->setSpecularColor(Color(3.1f, 3.2f, 3.3f, 3.4f));
   defaultLRP->setEmissiveColor(Color(4.1f, 4.2f, 4.3f, 4.4f));
   defaultLRP->setPower(5);

   CPPUNIT_ASSERT(lrp == *defaultLRP);

   delete defaultLRP;
}

//////////////////////////////////////////////////////////////////////////////

TEST(MaterialsParser, readingParitalLightReflectingPropertiesDefinition)
{
   ResourceStorage<Material> matStorage;
   MaterialsFactoryMock factory;
   MaterialsParser parser(factory, matStorage);

   parser.load("..\\Data\\MatWithPartialLRP.xml");

   Material& mat = matStorage.get("MatWithPartialLRP");
   const LightReflectingProperties& lrp = mat.getLightReflectingProperties();

   LightReflectingProperties* defaultLRP  = factory.createLightReflectingProperties();
   defaultLRP->setAmbientColor (Color(1.1f, 1.2f, 1.3f, 1.4f));
   defaultLRP->setDiffuseColor (Color());
   defaultLRP->setSpecularColor(Color(3.1f, 3.2f, 3.3f, 3.4f));
   defaultLRP->setEmissiveColor(Color());
   defaultLRP->setPower(5);

   CPPUNIT_ASSERT(lrp == *defaultLRP);

   delete defaultLRP;
}

//////////////////////////////////////////////////////////////////////////////

TEST(MaterialsParser, textureStage)
{
   ResourceStorage<Material> matStorage;
   MaterialsFactoryMock factory;
   MaterialsParser parser(factory, matStorage);
   parser.load("..\\Data\\SingleTextureStage.xml");

   Material& mat = matStorage.get("SingleTextureStage");

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, mat.getStagesCount());
   CPPUNIT_ASSERT_EQUAL(std::string("tex"), mat.getStage(0).getTexture().getName());
   CPPUNIT_ASSERT_EQUAL(MOP_ADD, mat.getStage(0).getColorOperation().getOperationCode());
   CPPUNIT_ASSERT_EQUAL(SC_TEXTURE, mat.getStage(0).getColorOperation().getArg1());
   CPPUNIT_ASSERT_EQUAL(SC_LRP, mat.getStage(0).getColorOperation().getArg2());
   CPPUNIT_ASSERT_EQUAL(MOP_MULTIPLY, mat.getStage(0).getAlphaOperation().getOperationCode());
   CPPUNIT_ASSERT_EQUAL(SC_TEXTURE, mat.getStage(0).getAlphaOperation().getArg1());
   CPPUNIT_ASSERT_EQUAL(SC_PREV, mat.getStage(0).getAlphaOperation().getArg2());
}

//////////////////////////////////////////////////////////////////////////////

TEST(MaterialsParser, multipleTextureStages)
{
   ResourceStorage<Material> matStorage;
   MaterialsFactoryMock factory;
   MaterialsParser parser(factory, matStorage);
   parser.load("..\\Data\\MultipleTextureStages.xml");

   Material& mat = matStorage.get("MultipleTextureStages");

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, mat.getStagesCount());

   // stage 0
   CPPUNIT_ASSERT_EQUAL(std::string("tex1"), mat.getStage(0).getTexture().getName());
   CPPUNIT_ASSERT_EQUAL(MOP_SUBTRACT, mat.getStage(0).getColorOperation().getOperationCode());
   CPPUNIT_ASSERT_EQUAL(SC_NONE, mat.getStage(0).getColorOperation().getArg1());
   CPPUNIT_ASSERT_EQUAL(SC_PREV, mat.getStage(0).getColorOperation().getArg2());
   CPPUNIT_ASSERT_EQUAL(MOP_MULTIPLY_ADD, mat.getStage(0).getAlphaOperation().getOperationCode());
   CPPUNIT_ASSERT_EQUAL(SC_LRP, mat.getStage(0).getAlphaOperation().getArg1());
   CPPUNIT_ASSERT_EQUAL(SC_TEXTURE, mat.getStage(0).getAlphaOperation().getArg2());

   // stage 1
   CPPUNIT_ASSERT_EQUAL(std::string("tex2"), mat.getStage(1).getTexture().getName());
   CPPUNIT_ASSERT_EQUAL(MOP_ADD_SIGNED, mat.getStage(1).getColorOperation().getOperationCode());
   CPPUNIT_ASSERT_EQUAL(SC_TEXTURE, mat.getStage(1).getColorOperation().getArg1());
   CPPUNIT_ASSERT_EQUAL(SC_LRP, mat.getStage(1).getColorOperation().getArg2());
   CPPUNIT_ASSERT_EQUAL(MOP_DISABLE, mat.getStage(1).getAlphaOperation().getOperationCode());
   CPPUNIT_ASSERT_EQUAL(SC_NONE, mat.getStage(1).getAlphaOperation().getArg1());
   CPPUNIT_ASSERT_EQUAL(SC_NONE, mat.getStage(1).getAlphaOperation().getArg2());
}

//////////////////////////////////////////////////////////////////////////////

TEST(MaterialsParser, multipleMaterialsWithTextureStages)
{
   ResourceStorage<Material> matStorage;
   MaterialsFactoryMock factory;
   MaterialsParser parser(factory, matStorage);
   parser.load("..\\Data\\MultMatsMultStages.xml");

   Material& mat1 = matStorage.get("Material_1");

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, mat1.getStagesCount());

   // stage 0
   CPPUNIT_ASSERT_EQUAL(std::string("tex1"), mat1.getStage(0).getTexture().getName());
   CPPUNIT_ASSERT_EQUAL(MOP_SUBTRACT, mat1.getStage(0).getColorOperation().getOperationCode());
   CPPUNIT_ASSERT_EQUAL(SC_NONE, mat1.getStage(0).getColorOperation().getArg1());
   CPPUNIT_ASSERT_EQUAL(SC_PREV, mat1.getStage(0).getColorOperation().getArg2());
   CPPUNIT_ASSERT_EQUAL(MOP_MULTIPLY_ADD, mat1.getStage(0).getAlphaOperation().getOperationCode());
   CPPUNIT_ASSERT_EQUAL(SC_LRP, mat1.getStage(0).getAlphaOperation().getArg1());
   CPPUNIT_ASSERT_EQUAL(SC_TEXTURE, mat1.getStage(0).getAlphaOperation().getArg2());

   // stage 1
   CPPUNIT_ASSERT_EQUAL(std::string("tex2"), mat1.getStage(1).getTexture().getName());
   CPPUNIT_ASSERT_EQUAL(MOP_ADD_SIGNED, mat1.getStage(1).getColorOperation().getOperationCode());
   CPPUNIT_ASSERT_EQUAL(SC_TEXTURE, mat1.getStage(1).getColorOperation().getArg1());
   CPPUNIT_ASSERT_EQUAL(SC_LRP, mat1.getStage(1).getColorOperation().getArg2());
   CPPUNIT_ASSERT_EQUAL(MOP_DISABLE, mat1.getStage(1).getAlphaOperation().getOperationCode());
   CPPUNIT_ASSERT_EQUAL(SC_NONE, mat1.getStage(1).getAlphaOperation().getArg1());
   CPPUNIT_ASSERT_EQUAL(SC_NONE, mat1.getStage(1).getAlphaOperation().getArg2());


   Material& mat2 = matStorage.get("Material_2");

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, mat2.getStagesCount());

   CPPUNIT_ASSERT_EQUAL(std::string("tex3"), mat2.getStage(0).getTexture().getName());
   CPPUNIT_ASSERT_EQUAL(MOP_ADD, mat2.getStage(0).getColorOperation().getOperationCode());
   CPPUNIT_ASSERT_EQUAL(SC_TEXTURE, mat2.getStage(0).getColorOperation().getArg1());
   CPPUNIT_ASSERT_EQUAL(SC_LRP, mat2.getStage(0).getColorOperation().getArg2());
   CPPUNIT_ASSERT_EQUAL(MOP_MULTIPLY, mat2.getStage(0).getAlphaOperation().getOperationCode());
   CPPUNIT_ASSERT_EQUAL(SC_TEXTURE, mat2.getStage(0).getAlphaOperation().getArg1());
   CPPUNIT_ASSERT_EQUAL(SC_PREV, mat2.getStage(0).getAlphaOperation().getArg2());
}

//////////////////////////////////////////////////////////////////////////////

#include "TestFramework.h"
#include "Material.h"
#include "LightReflectingPropertiesStub.h"
#include "TextureStub.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Material, equalityWithoutAnyComponents)
{
   TextureStub emptyTex("");
   Material mat1(emptyTex);
   Material mat2(emptyTex);

   CPPUNIT_ASSERT(mat1 == mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Material, equalityWithLightReflectancePropertiesForOneMatOnly)
{
   TextureStub emptyTex("");
   Material mat1(emptyTex);
   Material mat2(emptyTex);

   LightReflectingPropertiesStub lrp;
   mat1.setLightReflectingProperties(lrp);

   CPPUNIT_ASSERT(mat1 != mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Material, equalityWithTwoEqualLightReflectanceProperties)
{
   TextureStub emptyTex("");
   Material mat1(emptyTex);
   Material mat2(emptyTex);

   LightReflectingPropertiesStub lrp;

   mat1.setLightReflectingProperties(lrp);
   mat2.setLightReflectingProperties(lrp);

   CPPUNIT_ASSERT(mat1 == mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Material, equalityWithOneTextureOnly)
{
   TextureStub tex("tex1");
   TextureStub emptyTex("");
   Material mat1(tex);
   Material mat2(emptyTex);

   CPPUNIT_ASSERT(mat1 != mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Material, equalityWithTwoDifferentTextures)
{
   TextureStub tex1("tex1");
   TextureStub tex2("tex2");
   Material mat1(tex1);
   Material mat2(tex2);

   CPPUNIT_ASSERT(mat1 != mat2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Material, equalityWithTwoIdenticalTextures)
{
   TextureStub tex("tex");
   Material mat1(tex);
   Material mat2(tex);

   CPPUNIT_ASSERT(mat1 == mat2);
}

///////////////////////////////////////////////////////////////////////////////

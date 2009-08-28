#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Light.h"
#include "core\BoundingVolume.h"
#include "core\BoundingSphere.h"
#include "core\BoundingSpace.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Lights, boundingVolumeOfPointLight)
{
   Light light("light");
   light.setType(Light::LT_POINT);
   light.setRange(10);
   D3DXMatrixTranslation(&(light.accessLocalMtx()), 10, 20, 30);

   const BoundingSphere* bs = dynamic_cast<const BoundingSphere*>(&(light.getBoundingVolume()));
   CPPUNIT_ASSERT(NULL != bs);

   COMPARE_VEC(D3DXVECTOR3(10, 20, 30), bs->origin);
   CPPUNIT_ASSERT_EQUAL(10.f, bs->radius);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Lights, boundingVolumeOfDirectionalLight)
{
   Light light("light");
   light.setType(Light::LT_DIRECTIONAL);

   const BoundingSpace* bs = dynamic_cast<const BoundingSpace*>(&(light.getBoundingVolume()));
   CPPUNIT_ASSERT(NULL != bs);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Lights, boundingVolumeOfSpotLight)
{
   Light light("light");
   light.setType(Light::LT_SPOT);
   light.setRange(10); // we'll be using this range for all cases
   
   D3DXMATRIX direction;
   D3DXMATRIX translation;
   const BoundingSphere* bs = NULL;

   // check the light in its default position
   D3DXMatrixTranslation(&translation, 0, 0, 0);
   D3DXMatrixRotationYawPitchRoll(&direction, 0, 0, 0);
   light.accessLocalMtx() = translation * direction;

   bs = dynamic_cast<const BoundingSphere*>(&(light.getBoundingVolume()));
   CPPUNIT_ASSERT(NULL != bs);
   COMPARE_VEC(D3DXVECTOR3(0, 0, 5), bs->origin);
   CPPUNIT_ASSERT_EQUAL(5.f, bs->radius);

   // rotate the light a bit
   D3DXMatrixRotationYawPitchRoll(&direction, D3DXToRadian(90), 0, 0);
   light.accessLocalMtx() = translation * direction;

   bs = dynamic_cast<const BoundingSphere*>(&(light.getBoundingVolume()));
   CPPUNIT_ASSERT(NULL != bs);
   COMPARE_VEC(D3DXVECTOR3(5, 0, 0), bs->origin);
   CPPUNIT_ASSERT_EQUAL(5.f, bs->radius);

   // and move it around
   D3DXMatrixTranslation(&translation, 0, 10, 0);
   light.accessLocalMtx() = translation * direction;

   bs = dynamic_cast<const BoundingSphere*>(&(light.getBoundingVolume()));
   CPPUNIT_ASSERT(NULL != bs);
   COMPARE_VEC(D3DXVECTOR3(5, 10, 0), bs->origin);
   CPPUNIT_ASSERT_EQUAL(5.f, bs->radius);
}

///////////////////////////////////////////////////////////////////////////////

#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\MaterialInstance.h"
#include "core-Renderer\MaterialsDB.h"


///////////////////////////////////////////////////////////////////////////////

TEST( MaterialsDB, sameMaterialAlwaysAssignedTheSameIdx )
{
   MaterialsDB materialsDB;
   MaterialInstance mat1( FilePath( "mat1" ) );
   MaterialInstance mat2( FilePath( "mat2" ) );

   uint mat1Idx = materialsDB.registerMaterial( mat1 );
   uint mat2Idx = materialsDB.registerMaterial( mat2 );

   CPPUNIT_ASSERT_EQUAL( (uint)0, mat1Idx );
   CPPUNIT_ASSERT_EQUAL( (uint)1, mat2Idx );

   // re-add material 1 - it should get the very same material index
   mat1Idx = materialsDB.registerMaterial( mat1 );
   CPPUNIT_ASSERT_EQUAL( (uint)0, mat1Idx );

}

///////////////////////////////////////////////////////////////////////////////

#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Skeleton.h"
#include "core\Math.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Skeleton, changingShapeBindMatrix)
{
   Skeleton testSkeleton;

   Matrix testInvMtx;
   testInvMtx.setTranslation( Vector( 5, 0, 0 ) );
   testSkeleton.setTransformation( "bone", testInvMtx );

   COMPARE_MTX( testInvMtx, testSkeleton.getInvBindPoseMtx( "bone" ) );

   // first change 
   Matrix changedShapeBindMtx;
   changedShapeBindMtx.setTranslation( Vector( 0, 5, 0 ) );
   testSkeleton.setShapeBindMatrix( changedShapeBindMtx );

   testInvMtx.setTranslation( Vector( 5, 5, 0 ) );
   COMPARE_MTX( testInvMtx, testSkeleton.getInvBindPoseMtx( "bone" ) );

   // second change
   changedShapeBindMtx.setTranslation( Vector( 0, 0, 5 ) );
   testSkeleton.setShapeBindMatrix( changedShapeBindMtx );

   testInvMtx.setTranslation( Vector( 5, 0, 5 ) );
   COMPARE_MTX( testInvMtx, testSkeleton.getInvBindPoseMtx( "bone" ) );
}

///////////////////////////////////////////////////////////////////////////////

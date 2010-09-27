#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Skeleton.h"
#include "core\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Skeleton, changingShapeBindMatrix)
{
   Skeleton testSkeleton;

   D3DXMATRIX testInvMtx;
   D3DXMatrixTranslation( &testInvMtx, 5, 0, 0 );
   testSkeleton.setTransformation( "bone", testInvMtx );

   COMPARE_MTX( testInvMtx, testSkeleton.getInvBindPoseMtx( "bone" ) );

   // first change 
   D3DXMATRIX changedShapeBindMtx;
   D3DXMatrixTranslation( &changedShapeBindMtx, 0, 5, 0 );
   testSkeleton.setShapeBindMatrix( changedShapeBindMtx );

   D3DXMatrixTranslation( &testInvMtx, 5, 5, 0 );
   COMPARE_MTX( testInvMtx, testSkeleton.getInvBindPoseMtx( "bone" ) );

   // second change
   D3DXMatrixTranslation( &changedShapeBindMtx, 0, 0, 5 );
   testSkeleton.setShapeBindMatrix( changedShapeBindMtx );

   D3DXMatrixTranslation( &testInvMtx, 5, 0, 5 );
   COMPARE_MTX( testInvMtx, testSkeleton.getInvBindPoseMtx( "bone" ) );
}

///////////////////////////////////////////////////////////////////////////////

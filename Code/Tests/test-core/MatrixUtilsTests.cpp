#include "core-TestFramework\TestFramework.h"
#include "core\Matrix.h"
#include "core\MatrixUtils.h"
#include "core\MathDefs.h"
#include "core\Vector.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

TEST( MatrixUtils, generatePrespectiveProjection )
{
   Matrix tamyMtx;
   D3DXMATRIX dxMtx;

   float fov = DEG2RAD( 60.0f );
   float aspectRatio = 1.3333f;
   float nearZ = 1.01f;
   float farZ = 5000.0f;

   D3DXMatrixPerspectiveFovLH( &dxMtx, fov, aspectRatio, nearZ, farZ );
   MatrixUtils::generatePrespectiveProjection( fov, aspectRatio, nearZ, farZ, tamyMtx );
   COMPARE_MTX( dxMtx, tamyMtx );
}

///////////////////////////////////////////////////////////////////////////////

TEST( MatrixUtils, generateOrthogonalProjection )
{
   Matrix tamyMtx;
   D3DXMATRIX dxMtx;

   float width = 2.0f;
   float height = 2.0f;
   float nearZ = 1.01f;
   float farZ = 5000.0f;

   D3DXMatrixOrthoLH( &dxMtx, width, height, nearZ, farZ );
   MatrixUtils::generateOrthogonalProjection( width, height, nearZ, farZ, tamyMtx );
   COMPARE_MTX( dxMtx, tamyMtx );
}

///////////////////////////////////////////////////////////////////////////////

TEST( MatrixUtils, generateLookAtLH )
{
   Matrix tamyLookAtMtx;

   Vector cameraOriginPos( 10, 20, -30 ); 
   Vector lookAtPos( 15, 20, -30 );
   Vector upAxis; upAxis.set( Quad_0100 );

   MatrixUtils::generateLookAtLH( cameraOriginPos, lookAtPos, upAxis, tamyLookAtMtx );

   Vector expectedLookVec;
   expectedLookVec.setSub( lookAtPos, cameraOriginPos );
   expectedLookVec.normalize();

   Vector transformedLookVec;
   tamyLookAtMtx.transformNorm( Vector( 0, 0, 1 ), transformedLookVec );

   COMPARE_VEC( cameraOriginPos, tamyLookAtMtx.position() );
   COMPARE_VEC( expectedLookVec, transformedLookVec );
}

///////////////////////////////////////////////////////////////////////////////

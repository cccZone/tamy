#include "core-TestFramework\TestFramework.h"
#include "core\MatrixUtils.h"
#include "core\Vector.h"
#include "core\Matrix.h"
#include "core\EulerAngles.h"
#include "core\MathDefs.h"


///////////////////////////////////////////////////////////////////////////////

TEST( MatrixInterpolator, translation )
{
   Matrix start;
   Matrix end;
   Matrix expectedResult;

   start.setTranslation( Vector( 0, 0, 10 ) );
   end.setTranslation( Vector( 0, 0, 20 ) );

   Matrix result;

   expectedResult.setTranslation( Vector( 0, 0, 10 ) );
   MatrixUtils::lerp( start, end, 0, result );
   COMPARE_MTX( expectedResult, result );

   expectedResult.setTranslation( Vector( 0, 0, 20 ) );
   MatrixUtils::lerp( start, end, 1, result );
   COMPARE_MTX( expectedResult, result );

   expectedResult.setTranslation( Vector( 0, 0, 15 ) );
   MatrixUtils::lerp( start, end, 0.5f, result );
   COMPARE_MTX( expectedResult, result );
}

///////////////////////////////////////////////////////////////////////////////

TEST( MatrixInterpolator, rotation )
{
   Matrix start;
   Matrix end;

   start = Matrix::IDENTITY;
   end.setRotation( EulerAngles( 90, 0, 0 ) );

   Matrix result;

   MatrixUtils::lerp( start, end, 0, result );
   COMPARE_MTX( start, result );

   MatrixUtils::lerp( start, end, 1, result );
   COMPARE_MTX( end, result );

   Matrix expectedResult;
   expectedResult.setRotation( EulerAngles( 45, 0, 0 ) );
   MatrixUtils::lerp( start, end, 0.5f, result );
   COMPARE_MTX(expectedResult, result );
}

///////////////////////////////////////////////////////////////////////////////

TEST(WaypointCameraController, complexMovementx)
{
   Matrix start;
   start.setRotation( EulerAngles( 0, RAD2DEG( 10.0f ), RAD2DEG( -30.0f ) ) );

   Matrix end(  0.5f,      0,        -0.866025f,  0,
                0,         1,         0,          0,
                0.866025f, 0,         0.5f,       0,
               -5.980762f, 14.99999f, 24.999998f, 1 );

   Matrix result;
   MatrixUtils::lerp( start, end, 0, result );
   COMPARE_MTX( start, result );

   MatrixUtils::lerp( start, end, 1, result );
   COMPARE_MTX( end, result );
}

///////////////////////////////////////////////////////////////////////////////

#include "core-TestFramework\TestFramework.h"
#include "core\EulerAngles.h"
#include "core\Vector.h"
#include "core\Matrix.h"
#include "core\Quaternion.h"
#include "core\Plane.h"
#include "core\MathDefs.h"


///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, fromEulerAngles )
{
   D3DXMATRIX dxRotMtx;
   Matrix tamyRotMtx;
   EulerAngles angles;

   for ( float roll = -179.0f; roll <= 179.0f; roll += 15.0f )
   {
      for ( float yaw = -179.0f; yaw <= 179.0f; yaw += 15.0f )
      {
         for ( float pitch = -179.0f; pitch <= 179.0f; pitch += 15.0f )
         {
            D3DXMatrixRotationYawPitchRoll( &dxRotMtx, DEG2RAD(yaw), DEG2RAD(pitch), DEG2RAD(roll) );

            EulerAngles testAngle(yaw, pitch, roll);
            tamyRotMtx.setRotation( testAngle );
            COMPARE_MTX( dxRotMtx, tamyRotMtx );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, vectorTransformation )
{
   Matrix m;
   Vector result;

   // translations
   m.setTranslation( Vector( 1, 2, 3 ) );
   m.transform( Vector::ZERO, result );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(2.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(3.f, result.z, 1e-3);

   m.transform( Vector( 3, 2, 1 ), result );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(4.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(4.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(4.f, result.z, 1e-3);

   // rotations
   m.setRotation( EulerAngles(90.f, 0, 0) );
   m.transform( Vector::OZ, result );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.z, 1e-3);
}

///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, matrixConcatenation )
{
   Matrix m1, m2;
   m1.setRotation( EulerAngles(90.f, 0, 0) );
   m2.setTranslation( Vector( 1, 2, 3 ) );

   // compare the matrices - one concatenated using DX methods,
   // and the other using our method
   Matrix tamyResult;
   tamyResult.setMul( m2, m1 );

   D3DXMATRIX dxResult;
   D3DXMatrixMultiply( &dxResult, ( const D3DXMATRIX* )&m2, ( const D3DXMATRIX* )&m1 );
   COMPARE_MTX( tamyResult, dxResult );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, preMultiplyingSelf )
{
   Matrix m1, m2;
   m1.setRotation( EulerAngles(90.f, 0, 0) );
   m2.setTranslation( Vector( 1, 2, 3 ) );

   Matrix cleanMulResult;
   cleanMulResult.setMul( m2, m1 );

   m1.preMul( m2 );

   COMPARE_MTX( cleanMulResult, m1 );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, transformingSimpleCoordinate )
{
   Matrix m1, m2;
   m1.setRotation( EulerAngles(90.f, 0, 0) );
   m2.setTranslation( Vector( 1, 2, 3 ) );

   Matrix transformMtx;
   transformMtx.setMul( m2, m1 );

   // this transform will first translate a vector by ( 1, 2, 3 ) and then rotate it clockwise by 90 degrees

   D3DXVECTOR3 dxVecTransformed;
   D3DXVec3TransformCoord( &dxVecTransformed, ( const D3DXVECTOR3* )&Vector::OZ, ( const D3DXMATRIX* )&transformMtx );

   Vector tamyVecTransformed;
   transformMtx.transform( Vector::OZ, tamyVecTransformed );

   COMPARE_VEC( dxVecTransformed, tamyVecTransformed );

   CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.f, tamyVecTransformed.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(2.f, tamyVecTransformed.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.f, tamyVecTransformed.z, 1e-3);
}

///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, transformingSimpleNormal )
{
   Matrix mtx;

   // rotation
   {
      mtx.setRotation( EulerAngles(90.f, 0, 0) );

      Vector tamyNormalTransformed;
      mtx.transformNorm( Vector::OX, tamyNormalTransformed );

      D3DXVECTOR3 dxNormalTransformed;
      D3DXVec3TransformNormal( &dxNormalTransformed, ( const D3DXVECTOR3* )&Vector::OX, ( const D3DXMATRIX* )&mtx );

      COMPARE_VEC( dxNormalTransformed, tamyNormalTransformed );

      CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.f, tamyNormalTransformed.x, 1e-3 );
      CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.f, tamyNormalTransformed.y, 1e-3 );
      CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.f, tamyNormalTransformed.z, 1e-3 );
   }

   // translation
   {
      mtx.setTranslation( Vector( 0, 0, 10 ) );

      Vector tamyNormalTransformed;
      mtx.transformNorm( Vector::OX, tamyNormalTransformed );

      D3DXVECTOR3 dxNormalTransformed;
      D3DXVec3TransformNormal( &dxNormalTransformed, ( const D3DXVECTOR3* )&Vector::OX, ( const D3DXMATRIX* )&mtx );

      COMPARE_VEC( dxNormalTransformed, tamyNormalTransformed );

      CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.f, tamyNormalTransformed.x, 1e-3 );
      CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.f, tamyNormalTransformed.y, 1e-3 );
      CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.f, tamyNormalTransformed.z, 1e-3 );
   }
}


///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, transformingCoordinates )
{
   Matrix mtx;
   Vector result;

   mtx.setRotation( EulerAngles(90.f, 0, 0) ).transform( Vector::OZ, result );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.z, 1e-3);

   mtx.setRotation( EulerAngles(-90.f, 0, 0) ).transform( Vector::OZ, result );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.z, 1e-3);

   mtx.setRotation( EulerAngles(0, 90.f, 0) ).transform( Vector::OY, result );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.f, result.z, 1e-3);

   mtx.setTranslation( Vector(1, 2, 3) ).transform( Vector( 10, 20, 30 ), result );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(11.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(22.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(33.f, result.z, 1e-3);
}

///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, fromVectorAngle )
{
   Matrix mtx;
   Vector result;

   mtx.setRotation( EulerAngles(90.f, 0, 0) ).transform( Vector::OZ, result );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.z, 1e-3);

   mtx.setRotation( EulerAngles(-90.f, 0, 0) ).transform( Vector::OZ, result );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.z, 1e-3);

   mtx.setRotation( EulerAngles(0, 90.f, 0) ).transform( Vector::OY, result );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.f, result.z, 1e-3);

   mtx.setTranslation( Vector(1, 2, 3) ).transform( Vector( 10, 20, 30 ), result );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(11.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(22.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(33.f, result.z, 1e-3);
}

///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, directAccessToVectorComponents )
{
   Matrix mtx(
       1,  2,  3,  4,
       5,  6,  7,  8,
       9, 10, 11, 12,
      13, 14, 15, 16 );

   Vector expectedSideVec( 1, 2, 3, 4 );
   Vector expectedUpVec( 5, 6, 7, 8 );
   Vector expectedForwardVec( 9, 10, 11, 12 );
   Vector expectedPosVec( 13, 14, 15, 16 );

   const Vector& sideVec = mtx.sideVec();
   const Vector& upVec = mtx.upVec();
   const Vector& forwardVec = mtx.forwardVec();
   const Vector& posVec = mtx.position();
   
   COMPARE_VEC4( expectedSideVec, sideVec );
   COMPARE_VEC4( expectedUpVec, upVec );
   COMPARE_VEC4( expectedForwardVec, forwardVec );
   COMPARE_VEC4( expectedPosVec, posVec );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, invertingMatrix )
{
   Matrix m1, m2, mtx;
   m1.setTranslation( Vector( 1, 2, 3 ) );
   m2.setRotation( EulerAngles( 45.0f, 60.0f, -10.0f ) );
   mtx.setMul( m2, m1 );

   Matrix tamyInvertedMtx;
   tamyInvertedMtx.setInverse( mtx );

   D3DXMATRIX dxInvertedMtx;
   D3DXMatrixInverse( &dxInvertedMtx, NULL, ( const D3DXMATRIX* )&mtx );

   COMPARE_MTX( dxInvertedMtx, tamyInvertedMtx );

   // now check the self-inverted matrix
   mtx.invert();
   COMPARE_MTX( dxInvertedMtx, mtx );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, transposingMatrix )
{
   Matrix mtx(
      1,  2,  3,  4,
      5,  6,  7,  8,
      9, 10, 11, 12,
      13, 14, 15, 16 );

   Matrix tamyTransposedMtx;
   tamyTransposedMtx.setTransposed( mtx );

   D3DXMATRIX dxTransposedMtx;
   D3DXMatrixTranspose( &dxTransposedMtx, ( const D3DXMATRIX* )&mtx );

   COMPARE_MTX( dxTransposedMtx, tamyTransposedMtx );

   // now check the self-inverted matrix
   mtx.transpose();
   COMPARE_MTX( dxTransposedMtx, mtx );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, planeTransformation )
{
   Plane testPlane( 1, 0, 0, 10 );

   Matrix m1, m2, mtx;
   m1.setTranslation( Vector( 1, 2, 3 ) );
   m2.setRotation( EulerAngles( 45.0f, 60.0f, -10.0f ) );
   mtx.setMul( m2, m1 );

   Plane tamyTransformedPlane;
   mtx.transform( testPlane, tamyTransformedPlane );

   D3DXPLANE dxTransformedPlane;
   D3DXMATRIX dxMtx = ( const D3DXMATRIX& )mtx;
   D3DXMatrixInverse( &dxMtx, NULL, &dxMtx );
   D3DXMatrixTranspose( &dxMtx, &dxMtx );
   D3DXPlaneTransform( &dxTransformedPlane, ( const D3DXPLANE* )&testPlane, &dxMtx );

   COMPARE_PLANE( dxTransformedPlane, tamyTransformedPlane );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, setFromQuaternion )
{
   Vector arbitraryAxis( 0.3f, 0.5f, -0.2f );
   arbitraryAxis.normalize();

   Vector axis[] = { Vector::OX, Vector::OY, Vector::OZ, Vector::OX_NEG, Vector::OY_NEG, Vector::OZ_NEG, arbitraryAxis };
   int axisCount = 7;

   Quaternion q;
   Quaternion testQ;
   Matrix tamyMtx;
   D3DXMATRIX dxMtx;

   for ( int axisIdx = 0; axisIdx < axisCount; ++axisIdx )
   {
      for ( float angle = -179.0f; angle <= 179.0f; angle += 1.0f )
      {
         q.setAxisAngle( axis[axisIdx], DEG2RAD( angle ) );

         // test if the matrix generated by our method is the same as the one generated by DirectX
         tamyMtx.setRotation( q );
         D3DXMatrixRotationQuaternion( &dxMtx, ( const D3DXQUATERNION* )&q );
         COMPARE_MTX( dxMtx, tamyMtx );

         // and test the backwards operation
         tamyMtx.getRotation( testQ );
         COMPARE_QUAT( q, testQ );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

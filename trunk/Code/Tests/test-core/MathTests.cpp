#include "core-TestFramework\TestFramework.h"
#include "core\EulerAngles.h"
#include "core\Vector.h"
#include "core\Matrix.h"


///////////////////////////////////////////////////////////////////////////////

TEST(EulerAngles, normalization)
{
   EulerAngles o1(360, 720, -360);
   o1.normalize();
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o1.yaw, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o1.pitch, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o1.roll, 1e-3);

   EulerAngles o2(124, -91, -369);
   o2.normalize();
   CPPUNIT_ASSERT_DOUBLES_EQUAL(124.f, o2.yaw, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-91.f, o2.pitch, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-9.f, o2.roll, 1e-3);
}

///////////////////////////////////////////////////////////////////////////////

TEST(EulerAngles, comparisonOperatesOnNormalizedOrientation)
{
   EulerAngles o1(124, -96, -369);
   EulerAngles o2(124, -96, -9);
   CPPUNIT_ASSERT_EQUAL(o1, o2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(EulerAngles, angleBetweenTwoVectors)
{
   EulerAngles o(Vector(0, 0, 1), Vector(1, 0, 0));
   CPPUNIT_ASSERT_DOUBLES_EQUAL(90.f, o.yaw, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.pitch, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.roll, 1e-3);

   o = EulerAngles(Vector(0, 0, 1), Vector(-1, 0, 0));
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-90.f, o.yaw, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.pitch, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.roll, 1e-3);

   o = EulerAngles(Vector(0, 1, 0), Vector(0, 0, 1));
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.yaw, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(90.f, o.pitch, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.roll, 1e-1);

   o = EulerAngles(Vector(0, 1, 0), Vector(0, 0.5f, 0.5f));
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.yaw, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(45.f, o.pitch, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.roll, 1e-1);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Matrix, rotationMatrix)
{
   // lets make a matrix out of Euler angles
   EulerAngles o(124.f, -91.f, 0.f);
   Matrix mtx(o);

   D3DXMATRIX dxMtx;
   D3DXMatrixRotationYawPitchRoll( &dxMtx, DEG2RAD( o.yaw ), DEG2RAD( o.pitch ), DEG2RAD( o.roll ) );

   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[0][0], mtx.m[0][0], 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[1][0], mtx.m[0][1], 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[2][0], mtx.m[0][2], 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[3][0], mtx.m[0][3], 1e-3);

   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[0][1], mtx.m[1][0], 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[1][1], mtx.m[1][1], 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[2][1], mtx.m[1][2], 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[3][1], mtx.m[1][3], 1e-3);

   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[0][2], mtx.m[2][0], 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[1][2], mtx.m[2][1], 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[2][2], mtx.m[2][2], 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[3][2], mtx.m[2][3], 1e-3);

   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[0][3], mtx.m[3][0], 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[1][3], mtx.m[3][1], 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[2][3], mtx.m[3][2], 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(dxMtx.m[3][3], mtx.m[3][3], 1e-3);

   // and roll that operation back
   EulerAngles rollBack(mtx);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(124.f, rollBack.yaw, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-91.f, rollBack.pitch, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, rollBack.roll, 1e-3);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Matrix, matrixConcatenation)
{
   Matrix m1( EulerAngles(90.f, 0, 0) );
   Matrix m2( Vector( 1, 2, 3 ) );

   Matrix m = m2 * m1;
   Vector result = m.transform( Vector( 0, 0, 1 ) );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(2.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(2.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(3.f, result.z, 1e-3);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Matrix, transformingAVector)
{
   Vector result = Matrix( EulerAngles(90.f, 0, 0) ).transform( Vector( 0, 0, 1 ) );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.z, 1e-3);

   result = Matrix( EulerAngles(-90.f, 0, 0) ).transform( Vector( 0, 0, 1 ) );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.z, 1e-3);

   result = Matrix( EulerAngles(0, 90.f, 0) ).transform( Vector( 0, 1, 0 ) );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.f, result.z, 1e-3);

   result = Matrix( Vector(1, 2, 3) ).transform( Vector( 10, 20, 30 ) );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(11.f, result.x, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(22.f, result.y, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(33.f, result.z, 1e-3);
}

///////////////////////////////////////////////////////////////////////////////

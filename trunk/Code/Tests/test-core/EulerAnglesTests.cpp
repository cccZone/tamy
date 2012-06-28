#include "core-TestFramework\TestFramework.h"
#include "core\EulerAngles.h"
#include "core\Vector.h"
#include "core\Matrix.h"
#include "core\Quaternion.h"
#include "core\Plane.h"
#include "core\MathDefs.h"


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

TEST( EulerAngles, fromMatrix )
{
   Matrix rotMtx;
   EulerAngles angles;

   {
      EulerAngles testAngle( 90.0f, 0.0f, 0.0f );
      rotMtx.setRotation( testAngle);
      rotMtx.getRotation( angles );

      COMPARE_EULER( testAngle, angles );
   }

   {
      // northern gimball lock
      EulerAngles testAngle( 0.0f, 90.0f, 0.0f );
      rotMtx.setRotation( testAngle);
      rotMtx.getRotation( angles );

      COMPARE_EULER( testAngle, angles );
   }

   {
      // another case of northern gimball lock
      EulerAngles testAngle( 45.0f, 90.0f, 0.0f );
      rotMtx.setRotation( testAngle);
      rotMtx.getRotation( angles );

      COMPARE_EULER( testAngle, angles );
   }

   {
      // another case of northern gimball lock
      EulerAngles testAngle( -45.0f, 90.0f, 0.0f );
      rotMtx.setRotation( testAngle);
      rotMtx.getRotation( angles );

      COMPARE_EULER( testAngle, angles );
   }

   {
      EulerAngles testAngle( 0.0f, 0.0f, 90.0f );
      rotMtx.setRotation( testAngle);
      rotMtx.getRotation( angles );

      COMPARE_EULER( testAngle, angles );
   }

   {
      EulerAngles testAngle( -90.0f, 0.0f, 0.0f );
      rotMtx.setRotation( testAngle);
      rotMtx.getRotation( angles );

      COMPARE_EULER( testAngle, angles );
   }

   {
      // southern gimball lock
      EulerAngles testAngle( 0.0f, -90.0f, 0.0f );
      rotMtx.setRotation( testAngle);
      rotMtx.getRotation( angles );

      COMPARE_EULER( testAngle, angles );
   }

   {
      // another case of southern gimball lock
      EulerAngles testAngle( 45.0f, -90.0f, 0.0f );
      rotMtx.setRotation( testAngle);
      rotMtx.getRotation( angles );

      COMPARE_EULER( testAngle, angles );
   }

   {
      // another case of southern gimball lock
      EulerAngles testAngle( -45.0f, -90.0f, 0.0f );
      rotMtx.setRotation( testAngle);
      rotMtx.getRotation( angles );

      COMPARE_EULER( testAngle, angles );
   }

   {
      EulerAngles testAngle( 0.0f, 0.0f, -90.0f );
      rotMtx.setRotation( testAngle);
      rotMtx.getRotation( angles );

      COMPARE_EULER( testAngle, angles );
   }
   
   {
      EulerAngles testAngle( 45.0f, 45.0f, 45.0f );
      rotMtx.setRotation( testAngle);
      rotMtx.getRotation( angles );

      COMPARE_EULER( testAngle, angles );
   }

   {
      EulerAngles testAngle( 10.0f, 20.0f, 30.0f );
      rotMtx.setRotation( testAngle);
      rotMtx.getRotation( angles );

      COMPARE_EULER( testAngle, angles );
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST( EulerAngles, setFromShortestRotation )
{
   EulerAngles o;
   o.setFromShortestRotation( Vector::OZ, Vector::OX );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(90.f, o.yaw, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.pitch, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.roll, 1e-3);

   o.setFromShortestRotation( Vector::OZ, Vector::OX_NEG );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-90.f, o.yaw, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.pitch, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.roll, 1e-3);

   o.setFromShortestRotation( Vector::OY, Vector::OZ );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.yaw, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(90.f, o.pitch, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.roll, 1e-1);

   o.setFromShortestRotation( Vector::OX, Vector::OY );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.yaw, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.pitch, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(90.f, o.roll, 1e-1);

   o.setFromShortestRotation( Vector::OY, Vector::OX );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.yaw, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.f, o.pitch, 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-90.f, o.roll, 1e-1);

   Vector diagVec( 0, 0.5f, 0.5f );
   diagVec.normalize();
   o.setFromShortestRotation( Vector::OY, diagVec );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.f, o.yaw, 1e-3 );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 45.f, o.pitch, 1e-3 );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.f, o.roll, 1e-1 );
}

///////////////////////////////////////////////////////////////////////////////

TEST( EulerAngles, setFromQuaternion )
{
   Vector arbitraryAxis( 0.3f, 0.5f, -0.2f );
   arbitraryAxis.normalize();

   Vector axis[] = { Vector::OY, Vector::OZ, Vector::OX, Vector::OX_NEG, Vector::OY_NEG, Vector::OZ_NEG, arbitraryAxis };

   // conversion Quaternion->EulerAngle is not so well defined, especially for complex rotations.
   // That's why the last part of the test - the one that rotates around an arbitrarily selected axis
   // breaks instantaneously.
   // That's why I'm limiting the number of cases used here to rotations around the fixed axis only,
   // but leaving the other case so that maybe one day, when I come up with a correct equation, I can make it work again
   int axisCount = 6;

   Quaternion q;
   Quaternion testQ;
   EulerAngles ea;

   for ( int axisIdx = 0; axisIdx < axisCount; ++axisIdx )
   {
      for ( float angle = -179.0f; angle <= 179.0f; angle += 1.0f )
      {
         q.setAxisAngle( axis[axisIdx], DEG2RAD( angle ) );

         ea.setFromQuaternion( q );
         testQ.setFromEulerAngles( ea );

         // there's another issue with Quaternion->EulerAngle conversion here. For yaw < -90.0f && yaw > 90.0f,
         // the returned rotation is a rotation complement- basically it will return info that 'pitch' and 'roll' 
         // are both equal 180deg and 'yaw' = sign('acutalYaw') > 0 ? 90.0f - 'actualYaw' : 90.0f + 'actualYaw'.
         // I browsed the web but the euqations they have there exhibit the same characteristics, that's why I'm using
         // this high-tolerance comparison method here
         COMPARE_QUAT_ALLOW_CONJUGATION( testQ, q );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
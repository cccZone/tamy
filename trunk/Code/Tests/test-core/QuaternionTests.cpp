#include "core-TestFramework\TestFramework.h"
#include "core\EulerAngles.h"
#include "core\Vector.h"
#include "core\Matrix.h"
#include "core\Quaternion.h"
#include "core\Plane.h"
#include "core\MathDefs.h"


///////////////////////////////////////////////////////////////////////////////

TEST( Quaternion, fromAxisAngle )
{
   Vector arbitraryAxis( 0.3f, 0.5f, -0.2f );
   arbitraryAxis.normalize();

   Vector axis[] = { Vector::OX, Vector::OY, Vector::OZ, Vector::OX_NEG, Vector::OY_NEG, Vector::OZ_NEG, arbitraryAxis };
   int axisCount = 7;

   D3DXQUATERNION dxQuat;
   Quaternion tamyQuat;

   for ( int axisIdx = 0; axisIdx < axisCount; ++axisIdx )
   {
      for ( float angle = -179.0f; angle <= 179.0f; angle += 1.0f )
      {
         tamyQuat.setAxisAngle( axis[axisIdx], DEG2RAD( angle ) );
         D3DXQuaternionRotationAxis( &dxQuat, ( const D3DXVECTOR3* )&axis[axisIdx], DEG2RAD( angle ) );
         COMPARE_QUAT( dxQuat, tamyQuat );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST( Quaternion, retrievingAxisAngleValues )
{
   Vector arbitraryAxis( 0.3f, 0.5f, -0.2f );
   arbitraryAxis.normalize();

   Vector axis[] = { Vector::OX, Vector::OY, Vector::OZ, Vector::OX_NEG, Vector::OY_NEG, Vector::OZ_NEG, arbitraryAxis };
   int axisCount = 7;

   Quaternion quat;
   Vector retAxis;

   for ( int axisIdx = 0; axisIdx < axisCount; ++axisIdx )
   {
      for ( float angle = -179.0f; angle <= 179.0f; angle += 1.0f )
      {
         if ( angle == 0.0f )
         {
            // the method doesn't work for angles == 0
            continue;
         }
         quat.setAxisAngle( axis[axisIdx], DEG2RAD( angle ) );

         float retAngle = RAD2DEG( quat.getAngle() );
         float signChange = retAngle * angle > 0.0f ? 1.0f : -1.0f;

         CPPUNIT_ASSERT_DOUBLES_EQUAL( abs( angle ), retAngle, 1e-3 );

         quat.getAxis( retAxis );
         retAxis.mul( signChange );
         COMPARE_VEC( axis[axisIdx], retAxis );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST( Quaternion, angleDecomposition )
{
   Quaternion quat;
	Quaternion remainingQuat;
	float angle;

	{
		quat.setAxisAngle( Vector::OX, DEG2RAD( 90.0f ) );

		// a decomposition along the creation axis should not leave any quaternion behind
		{
			angle = quat.decompose( Vector::OX, remainingQuat );

			COMPARE_QUAT( remainingQuat, Quaternion::IDENTITY );
			CPPUNIT_ASSERT_DOUBLES_EQUAL( 90.0f, RAD2DEG( angle ), 1e-3 );
		}

		// a decomposition along an orthogonal axis should leave the entire quaternion untouched
		// and return angle == 0
		{
			angle = quat.decompose( Vector::OY, remainingQuat );

			COMPARE_QUAT( remainingQuat, quat );
			CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0395f, RAD2DEG( angle ), 1e-3 );
		}
	}

	quat.setAxisAngle( Vector::OX_NEG, DEG2RAD( 45.0f ) );
	{
		// complete removal
		{
			angle = quat.decompose( Vector::OX_NEG, remainingQuat );

			COMPARE_QUAT( remainingQuat, Quaternion::IDENTITY );
			CPPUNIT_ASSERT_DOUBLES_EQUAL( 45.0f, RAD2DEG( angle ), 1e-3 );
		}

		// null removal
		{
			angle = quat.decompose( Vector::OY, remainingQuat );

			COMPARE_QUAT( remainingQuat, quat );
			CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0395f, RAD2DEG( angle ), 1e-3 );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST( Quaternion, setFromShortestRotation )
{
   Quaternion quat;

   quat.setFromShortestRotation( Vector::OZ, Vector::OX );
   COMPARE_QUAT_AXIS_ANGLE( quat, Vector::OY, 90.0f );   

   quat.setFromShortestRotation( Vector::OZ, Vector::OX_NEG );
   COMPARE_QUAT_AXIS_ANGLE( quat, Vector::OY_NEG, 90.0f );

   quat.setFromShortestRotation( Vector::OY, Vector::OZ );
   COMPARE_QUAT_AXIS_ANGLE( quat, Vector::OX, 90.0f );

   quat.setFromShortestRotation( Vector::OX, Vector::OY );
   COMPARE_QUAT_AXIS_ANGLE( quat, Vector::OZ, 90.0f );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Quaternion, setFromEulerAngles )
{
   Quaternion tamyQuat;
   D3DXQUATERNION dxQuat;
   EulerAngles ea;

   for ( float yaw = -179.0f; yaw <= 179.0f; yaw += 15.0f )
   {
      for ( float pitch = -179.0f; pitch <= 179.0f; pitch += 15.0f )
      {
         for ( float roll = -179.0f; roll <= 179.0f; roll += 15.0f )
         {
            ea.yaw = yaw;
            ea.pitch = pitch;
            ea.roll = roll;

            tamyQuat.setFromEulerAngles( ea );
            D3DXQuaternionRotationYawPitchRoll( &dxQuat, DEG2RAD( ea.yaw ), DEG2RAD( ea.pitch ), DEG2RAD( ea.roll ) ); 

            COMPARE_QUAT( dxQuat, tamyQuat );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST( Quaternion, slerp )
{
   Quaternion q1, q2, resultQ;
   q1.setAxisAngle( Vector::OY, 0.0f );
   q2.setAxisAngle( Vector::OY, DEG2RAD( 90.0f ) );

   resultQ.setSlerp( q1, q2, 0.0f );
   COMPARE_QUAT( q1, resultQ );

   resultQ.setSlerp( q1, q2, 1.0f );
   COMPARE_QUAT( q2, resultQ );

   resultQ.setSlerp( q1, q2, 0.5f );
   COMPARE_QUAT_AXIS_ANGLE( resultQ, Vector::OY, 45.0f );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Quaternion, transform )
{
   Quaternion q;
   Vector result;

   q.setAxisAngle( Vector::OY, DEG2RAD( 90.0f ) );
   q.transform( Vector::OZ, result );
   COMPARE_VEC( Vector::OX, result );

   q.setAxisAngle( Vector::OX, DEG2RAD( 90.0f ) );
   q.transform( Vector::OY, result );
   COMPARE_VEC( Vector::OZ, result );

   q.setAxisAngle( Vector::OZ, DEG2RAD( 90.0f ) );
   q.transform( Vector::OX, result );
   COMPARE_VEC( Vector::OY, result );

   q.setAxisAngle( Vector::OZ_NEG, DEG2RAD( 90.0f ) );
   q.transform( Vector::OX, result );
   COMPARE_VEC( Vector::OY_NEG, result );
}

///////////////////////////////////////////////////////////////////////////////

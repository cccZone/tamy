#include "core-TestFramework\TestFramework.h"
#include "core\Math.h"


///////////////////////////////////////////////////////////////////////////////

TEST( AABoundingBox, transformation )
{
   AABoundingBox centralBB( Vector( -1, -1, -1 ), Vector( 1, 1, 1 ) );

   Transform transform;
   Matrix transformMtx;
   AABoundingBox transformedBB;

   {
      transform.m_rotation.setAxisAngle( Vector::OZ, DEG2RAD( 45.0f ) );
      transform.toMatrix( transformMtx );
      centralBB.transform( transformMtx, transformedBB );

      COMPARE_VEC( Vector( -1.4142135f, -1.4142135f, -1 ), transformedBB.min );
      COMPARE_VEC( Vector(  1.4142135f,  1.4142135f,  1 ), transformedBB.max );
   }

   {
      transform.m_rotation.setAxisAngle( Vector::OX, DEG2RAD( 45.0f ) );
      transform.toMatrix( transformMtx );
      centralBB.transform( transformMtx, transformedBB );

      COMPARE_VEC( Vector( -1, -1.4142135f, -1.4142135f ), transformedBB.min );
      COMPARE_VEC( Vector(  1,  1.4142135f,  1.4142135f ), transformedBB.max );
   }

   {
      transform.m_rotation.setAxisAngle( Vector::OY, DEG2RAD( 45.0f ) );
      transform.toMatrix( transformMtx );
      centralBB.transform( transformMtx, transformedBB );

      COMPARE_VEC( Vector( -1.4142135f, -1, -1.4142135f ), transformedBB.min );
      COMPARE_VEC( Vector(  1.4142135f,  1,  1.4142135f ), transformedBB.max );
   }
}

///////////////////////////////////////////////////////////////////////////////

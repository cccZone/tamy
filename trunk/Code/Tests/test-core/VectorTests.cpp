#include "core-TestFramework\TestFramework.h"
#include "core\Vector.h"
#include "core\MathDefs.h"


///////////////////////////////////////////////////////////////////////////////

TEST( Vector, crossProduct )
{
   Vector tamyCross;
   D3DXVECTOR3 dxCross;

   tamyCross.setCross( Vector::OX, Vector::OZ );
   D3DXVec3Cross( &dxCross, ( const D3DXVECTOR3* )&Vector::OX, ( const D3DXVECTOR3* )&Vector::OZ );
   COMPARE_VEC( dxCross, tamyCross );

   tamyCross.setCross( Vector::OY, Vector::OX_NEG );
   D3DXVec3Cross( &dxCross, ( const D3DXVECTOR3* )&Vector::OY, ( const D3DXVECTOR3* )&Vector::OX_NEG );
   COMPARE_VEC( dxCross, tamyCross );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Vector, lerp )
{
   Vector start( 1, 2, 3 );
   Vector end( 2, 4, 6 );
 
   Vector result;

   result.setLerp( start, end, 0.0f );
   COMPARE_VEC( start, result );

   result.setLerp( start, end, 1.0f );
   COMPARE_VEC( end, result );

   result.setLerp( start, end, 0.5f );
   COMPARE_VEC( Vector( 1.5f, 3.f, 4.5f ), result );
}

///////////////////////////////////////////////////////////////////////////////

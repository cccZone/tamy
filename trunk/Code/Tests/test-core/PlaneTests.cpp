#include "core-TestFramework\TestFramework.h"
#include "core\Plane.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

TEST( Plane, setFromPointNormal )
{
   Plane tamyPlane;
   D3DXPLANE dxPlane;
   Vector point, normal;
   
   point = Vector( 10, 20, 30 );
   normal = Vector::OX;
   D3DXPlaneFromPointNormal( &dxPlane, ( const D3DXVECTOR3* )&point, ( const D3DXVECTOR3* )&normal );
   tamyPlane.setFromPointNormal( point, normal );
   COMPARE_PLANE( dxPlane, tamyPlane );

   point = Vector( -4, 0, 5.5f );
   normal = Vector::OY;
   D3DXPlaneFromPointNormal( &dxPlane, ( const D3DXVECTOR3* )&point, ( const D3DXVECTOR3* )&normal );
   tamyPlane.setFromPointNormal( point, normal );
   COMPARE_PLANE( dxPlane, tamyPlane );

   point = Vector( 0, 0, -20.0f );
   normal = Vector::OZ;
   D3DXPlaneFromPointNormal( &dxPlane, ( const D3DXVECTOR3* )&point, ( const D3DXVECTOR3* )&normal );
   tamyPlane.setFromPointNormal( point, normal );
   COMPARE_PLANE( dxPlane, tamyPlane );

   point = Vector( 10, 20, 30 );
   normal = Vector::OX_NEG;
   D3DXPlaneFromPointNormal( &dxPlane, ( const D3DXVECTOR3* )&point, ( const D3DXVECTOR3* )&normal );
   tamyPlane.setFromPointNormal( point, normal );
   COMPARE_PLANE( dxPlane, tamyPlane );

   point = Vector( -4, 0, 5.5f );
   normal = Vector::OY_NEG;
   D3DXPlaneFromPointNormal( &dxPlane, ( const D3DXVECTOR3* )&point, ( const D3DXVECTOR3* )&normal );
   tamyPlane.setFromPointNormal( point, normal );
   COMPARE_PLANE( dxPlane, tamyPlane );

   point = Vector( 0, 0, -20.0f );
   normal = Vector::OZ_NEG;
   D3DXPlaneFromPointNormal( &dxPlane, ( const D3DXVECTOR3* )&point, ( const D3DXVECTOR3* )&normal );
   tamyPlane.setFromPointNormal( point, normal );
   COMPARE_PLANE( dxPlane, tamyPlane );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Plane, setFromPoints )
{
   Plane tamyPlane;
   D3DXPLANE dxPlane;
   Vector ptA, ptB, ptC;

   ptA.set( 0, 0, 0 ); ptB.set( 10, 0, 0 ); ptC.set( 0, 10, 0 );
   D3DXPlaneFromPoints( &dxPlane, ( const D3DXVECTOR3* )&ptA, ( const D3DXVECTOR3* )&ptB, ( const D3DXVECTOR3* )&ptC );
   tamyPlane.setFromPoints( ptA, ptB, ptC );
   COMPARE_PLANE( dxPlane, tamyPlane );

   ptA.set( 0, 0, 0 ); ptB.set( 10, 0, 0 ); ptC.set( 0, 0, 10 );
   D3DXPlaneFromPoints( &dxPlane, ( const D3DXVECTOR3* )&ptA, ( const D3DXVECTOR3* )&ptB, ( const D3DXVECTOR3* )&ptC );
   tamyPlane.setFromPoints( ptA, ptB, ptC );
   COMPARE_PLANE( dxPlane, tamyPlane );

   ptA.set( 0, 0, 0 ); ptB.set( 0, 10, 0 ); ptC.set( 0, 0, 10 );
   D3DXPlaneFromPoints( &dxPlane, ( const D3DXVECTOR3* )&ptA, ( const D3DXVECTOR3* )&ptB, ( const D3DXVECTOR3* )&ptC );
   tamyPlane.setFromPoints( ptA, ptB, ptC );
   COMPARE_PLANE( dxPlane, tamyPlane );

   ptA.set( 0, 0, 0 ); ptB.set( 3, 0, 0 ); ptC.set( 0, -4, 0 );
   D3DXPlaneFromPoints( &dxPlane, ( const D3DXVECTOR3* )&ptA, ( const D3DXVECTOR3* )&ptB, ( const D3DXVECTOR3* )&ptC );
   tamyPlane.setFromPoints( ptA, ptB, ptC );
   COMPARE_PLANE( dxPlane, tamyPlane );

   ptA.set( 0, 0, 0 ); ptB.set( -10, 0, 0 ); ptC.set( 0, 0, -5 );
   D3DXPlaneFromPoints( &dxPlane, ( const D3DXVECTOR3* )&ptA, ( const D3DXVECTOR3* )&ptB, ( const D3DXVECTOR3* )&ptC );
   tamyPlane.setFromPoints( ptA, ptB, ptC );
   COMPARE_PLANE( dxPlane, tamyPlane );

   ptA.set( 0, 0, 0 ); ptB.set( 0, -10, 0 ); ptC.set( 0, 0, -10 );
   D3DXPlaneFromPoints( &dxPlane, ( const D3DXVECTOR3* )&ptA, ( const D3DXVECTOR3* )&ptB, ( const D3DXVECTOR3* )&ptC );
   tamyPlane.setFromPoints( ptA, ptB, ptC );
   COMPARE_PLANE( dxPlane, tamyPlane );

   ptA.set( 20, 3, -4 ); ptB.set( 5, -10, 1 ); ptC.set( 2, -1, -10 );
   D3DXPlaneFromPoints( &dxPlane, ( const D3DXVECTOR3* )&ptA, ( const D3DXVECTOR3* )&ptB, ( const D3DXVECTOR3* )&ptC );
   tamyPlane.setFromPoints( ptA, ptB, ptC );
   COMPARE_PLANE( dxPlane, tamyPlane );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Plane, normalize )
{
   Plane tamyPlane;
   D3DXPLANE dxPlane;

   tamyPlane.set( 1, 2, -3, 10 );
   D3DXPlaneNormalize( &dxPlane, ( const D3DXPLANE* )&tamyPlane );
   tamyPlane.normalize();
   COMPARE_PLANE( dxPlane, tamyPlane );

   tamyPlane.set( -1, -1, 0, -1 );
   D3DXPlaneNormalize( &dxPlane, ( const D3DXPLANE* )&tamyPlane );
   tamyPlane.normalize();
   COMPARE_PLANE( dxPlane, tamyPlane );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Plane, dotCoord )
{
   Plane plane;
   Vector testPt;
   
   testPt.set( 3, 2, -1 );
   plane.set( 1, 0, 0, 10 );
   float dxDot = D3DXPlaneDotCoord( ( const D3DXPLANE* )&plane, ( const D3DXVECTOR3* )&testPt );
   float tamyDot = plane.dotCoord( testPt );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( dxDot, tamyDot, 1e-3f );

   testPt.set( 3, 2, -1 );
   plane.set( 0, -1, 0, -12 );
   dxDot = D3DXPlaneDotCoord( ( const D3DXPLANE* )&plane, ( const D3DXVECTOR3* )&testPt );
   tamyDot = plane.dotCoord( testPt );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( dxDot, tamyDot, 1e-3f );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Plane, dotNormal )
{
   Plane plane;
   Vector testPt;

   testPt.set( 3, 2, -1 );
   plane.set( 1, 0, 0, 10 );
   float dxDot = D3DXPlaneDotNormal( ( const D3DXPLANE* )&plane, ( const D3DXVECTOR3* )&testPt );
   float tamyDot = plane.dotNormal( testPt );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( dxDot, tamyDot, 1e-3f );

   testPt.set( 3, 2, -1 );
   plane.set( 0, -1, 0, -12 );
   dxDot = D3DXPlaneDotNormal( ( const D3DXPLANE* )&plane, ( const D3DXVECTOR3* )&testPt );
   tamyDot = plane.dotNormal( testPt );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( dxDot, tamyDot, 1e-3f );
}

///////////////////////////////////////////////////////////////////////////////

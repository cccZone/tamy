#include "core-TestFramework\TestFramework.h"
#include "core\Vector.h"
#include "core\Color.h"
#include "core\Matrix.h"


///////////////////////////////////////////////////////////////////////////////

TEST( Vector3, exchangingDataWithDX )
{
   Vector dxVec( 1, 2, 3 );
   const Vector& castDxVec = ( const Vector& )(dxVec);
   CPPUNIT_ASSERT_EQUAL( 1.0f, castDxVec.x );
   CPPUNIT_ASSERT_EQUAL( 2.0f, castDxVec.y );
   CPPUNIT_ASSERT_EQUAL( 3.0f, castDxVec.z );

   // now go the other way around
   Vector tamyVec( 5, 6, 7 );
   const Vector& castTamyVec = ( const Vector& )(tamyVec);
   CPPUNIT_ASSERT_EQUAL( 5.0f, castTamyVec.x );
   CPPUNIT_ASSERT_EQUAL( 6.0f, castTamyVec.y );
   CPPUNIT_ASSERT_EQUAL( 7.0f, castTamyVec.z );
}

// TODO: an array of Vector 

///////////////////////////////////////////////////////////////////////////////

TEST( Vector4, exchangingDataWithDX )
{
   D3DXVECTOR4 dxVec( 1, 2, 3, 4 );
   const Vector& castDxVec = (const Vector& )(dxVec);
   CPPUNIT_ASSERT_EQUAL( 1.0f, castDxVec.x );
   CPPUNIT_ASSERT_EQUAL( 2.0f, castDxVec.y );
   CPPUNIT_ASSERT_EQUAL( 3.0f, castDxVec.z );
   CPPUNIT_ASSERT_EQUAL( 4.0f, castDxVec.w );

   // now go the other way around
   Vector tamyVec( 5, 6, 7, 8 );
   const D3DXVECTOR4& castTamyVec = ( const D3DXVECTOR4& )(tamyVec);
   CPPUNIT_ASSERT_EQUAL( 5.0f, castTamyVec.x );
   CPPUNIT_ASSERT_EQUAL( 6.0f, castTamyVec.y );
   CPPUNIT_ASSERT_EQUAL( 7.0f, castTamyVec.z );
   CPPUNIT_ASSERT_EQUAL( 8.0f, castTamyVec.w );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Matrix, exchangingDataWithDX )
{
   Matrix dxMtx = Matrix::IDENTITY;
   const Matrix& castDxMtx = (const Matrix&)(dxMtx);
   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         CPPUNIT_ASSERT_EQUAL( dxMtx.m[row][col], castDxMtx.m[row][col] );
      }
   }

   // now go the other way around
   Matrix tamyMtx = Matrix::IDENTITY;
   const Matrix& castTamyMtx = (const Matrix&)(tamyMtx);
   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         CPPUNIT_ASSERT_EQUAL( tamyMtx.m[row][col], castTamyMtx.m[row][col] );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST( Color, exchangingDataWithDX )
{
   D3DXVECTOR4 dxColor( 0.1f, 0.2f, 0.3f, 0.4f );
   const Color& castDxColor = (const Color& )(dxColor);
   CPPUNIT_ASSERT_EQUAL( 0.1f, castDxColor.r );
   CPPUNIT_ASSERT_EQUAL( 0.2f, castDxColor.g );
   CPPUNIT_ASSERT_EQUAL( 0.3f, castDxColor.b );
   CPPUNIT_ASSERT_EQUAL( 0.4f, castDxColor.a );

   // now go the other way around
   Color tamyColor( 0.5f, 0.6f, 0.7f, 0.8f );
   const D3DXVECTOR4& castTamyVec = ( const D3DXVECTOR4& )(tamyColor);
   CPPUNIT_ASSERT_EQUAL( 0.5f, castTamyVec.x );
   CPPUNIT_ASSERT_EQUAL( 0.6f, castTamyVec.y );
   CPPUNIT_ASSERT_EQUAL( 0.7f, castTamyVec.z );
   CPPUNIT_ASSERT_EQUAL( 0.8f, castTamyVec.w );
}

///////////////////////////////////////////////////////////////////////////////

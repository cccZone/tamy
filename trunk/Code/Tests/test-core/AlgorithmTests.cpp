#include "core-TestFramework\TestFramework.h"
#include "core\Algorithms.h"


///////////////////////////////////////////////////////////////////////////////

TEST( Algorithms, clamp )
{
   CPPUNIT_ASSERT_EQUAL( 0.5f, clamp< float >( 0.5f, 0.0f, 1.0f ) );
   CPPUNIT_ASSERT_EQUAL( 0.0f, clamp< float >( -0.5f, 0.0f, 1.0f ) );
   CPPUNIT_ASSERT_EQUAL( 0.2f, clamp< float >( 0.1f, 0.2f, 1.0f ) );
   CPPUNIT_ASSERT_EQUAL( 1.0f, clamp< float >( 1.1f, 0.2f, 1.0f ) );
}

///////////////////////////////////////////////////////////////////////////////

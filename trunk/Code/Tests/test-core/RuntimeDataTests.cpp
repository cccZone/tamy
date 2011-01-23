#include "core-TestFramework\TestFramework.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

   struct MockObj
   {
      int      m_val;
      MockObj( int val = 0 ) : m_val( val ) {}
   };

}  // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST( RuntimeData, simpleTypes )
{
   RuntimeDataBuffer buffer1;
   RuntimeDataBuffer buffer2;

   TRuntimeVar< int > val;
   buffer1.registerVar( val );
   buffer2.registerVar( val );

   buffer1[val] = 2;
   buffer2[val] = 4;

   CPPUNIT_ASSERT_EQUAL( 2, buffer1[val] );
   CPPUNIT_ASSERT_EQUAL( 4, buffer2[val] );
}

///////////////////////////////////////////////////////////////////////////////

TEST( RuntimeData, pointers )
{
   RuntimeDataBuffer buffer1;
   RuntimeDataBuffer buffer2;

   TRuntimeVar< MockObj* > val;
   buffer1.registerVar( val );
   buffer2.registerVar( val );

   buffer1[val] = new MockObj( 2 );
   buffer2[val] = new MockObj( 4 );

   CPPUNIT_ASSERT_EQUAL( 2, buffer1[val]->m_val );
   CPPUNIT_ASSERT_EQUAL( 4, buffer2[val]->m_val );

   delete buffer1[val];
   delete buffer2[val];
}

///////////////////////////////////////////////////////////////////////////////

TEST( RuntimeData, complexTypes )
{
   RuntimeDataBuffer buffer1;
   RuntimeDataBuffer buffer2;

   TRuntimeVar< MockObj > val;
   buffer1.registerVar( val );
   buffer2.registerVar( val );

   buffer1[val].m_val = 2;
   buffer2[val].m_val = 4;

   CPPUNIT_ASSERT_EQUAL( 2, buffer1[val].m_val );
   CPPUNIT_ASSERT_EQUAL( 4, buffer2[val].m_val );
}

///////////////////////////////////////////////////////////////////////////////

TEST( RuntimeData, immutableTypes )
{
   RuntimeDataBuffer buffer;

   TRuntimeVar< std::string > val;
   buffer.registerVar( val );

   buffer[val] = "dupa";
   CPPUNIT_ASSERT_EQUAL( std::string( "dupa" ), buffer[val] );
}

///////////////////////////////////////////////////////////////////////////////

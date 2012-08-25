#include "core-TestFramework\TestFramework.h"
#include "core\List.h"
#include "core\MemoryPoolAllocator.h"


///////////////////////////////////////////////////////////////////////////////

TEST(List, regularOperations)
{
   List< int > l;

   l.pushBack(1);
   CPPUNIT_ASSERT_EQUAL( 1, l.back() );
   CPPUNIT_ASSERT_EQUAL( 1, l.front() );

   l.pushBack(2);
   CPPUNIT_ASSERT_EQUAL( 2, l.back() );
   CPPUNIT_ASSERT_EQUAL( 1, l.front() );

   l.pushBack(3);
   CPPUNIT_ASSERT_EQUAL( 3, l.back() );
   CPPUNIT_ASSERT_EQUAL( 1, l.front() );

   l.pushFront(0);
   CPPUNIT_ASSERT_EQUAL( 3, l.back() );
   CPPUNIT_ASSERT_EQUAL( 0, l.front() );

   l.popFront();
   CPPUNIT_ASSERT_EQUAL( 3, l.back() );
   CPPUNIT_ASSERT_EQUAL( 1, l.front() );

   l.popBack();
   CPPUNIT_ASSERT_EQUAL( 2, l.back() );
   CPPUNIT_ASSERT_EQUAL( 1, l.front() );
}

///////////////////////////////////////////////////////////////////////////////

TEST(List, customAllocator)
{
   MemoryPool pool( 256 );
   MemoryPoolAllocator allocator( &pool );
   List< int, MemoryPoolAllocator > l( &allocator );

   l.pushBack(1);
   CPPUNIT_ASSERT_EQUAL( 1, l.back() );
   CPPUNIT_ASSERT_EQUAL( 1, l.front() );

   l.pushBack(2);
   CPPUNIT_ASSERT_EQUAL( 2, l.back() );
   CPPUNIT_ASSERT_EQUAL( 1, l.front() );

   l.pushBack(3);
   CPPUNIT_ASSERT_EQUAL( 3, l.back() );
   CPPUNIT_ASSERT_EQUAL( 1, l.front() );

   l.pushFront(0);
   CPPUNIT_ASSERT_EQUAL( 3, l.back() );
   CPPUNIT_ASSERT_EQUAL( 0, l.front() );

   l.popFront();
   CPPUNIT_ASSERT_EQUAL( 3, l.back() );
   CPPUNIT_ASSERT_EQUAL( 1, l.front() );

   l.popBack();
   CPPUNIT_ASSERT_EQUAL( 2, l.back() );
   CPPUNIT_ASSERT_EQUAL( 1, l.front() );
}

///////////////////////////////////////////////////////////////////////////////

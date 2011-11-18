#include "core-TestFramework\TestFramework.h"
#include "core/MemoryPool.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class MemoryPoolObjectMock : public MemoryPoolObject
   {
   public:
      static int        s_instancesCount;

   private:
      int               m_val;

   public:
      MemoryPoolObjectMock( int val ) 
         : m_val( val ) 
      {
         ++s_instancesCount;
      }
      ~MemoryPoolObjectMock()
      {
         --s_instancesCount;
      }

      inline int getVal() const { return m_val; }
   };

   int MemoryPoolObjectMock::s_instancesCount = 0;

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST( MemoryPool, simpleAllocationsAndDeallocations )
{
   MemoryPool pool( 512 );

   CPPUNIT_ASSERT_EQUAL( 0, MemoryPoolObjectMock::s_instancesCount );

   MemoryPoolObject* obj = new ( pool ) MemoryPoolObjectMock( 1 );
   CPPUNIT_ASSERT_EQUAL( 1, dynamic_cast< MemoryPoolObjectMock* >(obj)->getVal() );
   CPPUNIT_ASSERT_EQUAL( 1, MemoryPoolObjectMock::s_instancesCount );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)1, pool.getAllocationsCount() );
   MEMPOOL_DELETE( obj );
   CPPUNIT_ASSERT_EQUAL( 0, MemoryPoolObjectMock::s_instancesCount );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)0, pool.getAllocationsCount() );

   obj = new ( pool ) MemoryPoolObjectMock( 2 ); 
   CPPUNIT_ASSERT_EQUAL( 2, dynamic_cast< MemoryPoolObjectMock* >(obj)->getVal() );
   CPPUNIT_ASSERT_EQUAL( 1, MemoryPoolObjectMock::s_instancesCount );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)1, pool.getAllocationsCount() );
   MEMPOOL_DELETE( obj );
   CPPUNIT_ASSERT_EQUAL( 0, MemoryPoolObjectMock::s_instancesCount );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)0, pool.getAllocationsCount() );
}

///////////////////////////////////////////////////////////////////////////////

TEST( MemoryPool, poolReset )
{
   MemoryPool pool( 512 );

   CPPUNIT_ASSERT_EQUAL( 0, MemoryPoolObjectMock::s_instancesCount );
 
   MemoryPoolObject* obj1 = new ( pool ) MemoryPoolObjectMock( 1 );
   MemoryPoolObject* obj2 = new ( pool ) MemoryPoolObjectMock( 2 );

   long ptr1 = (long)obj1;
   long ptr2 = (long)obj2;

   CPPUNIT_ASSERT_EQUAL( (unsigned int)2, pool.getAllocationsCount() );

   // compare the pointers - the two objects should not have the same allocation address
   CPPUNIT_ASSERT( ptr1 != ptr2 );

   // delete the objects
   MEMPOOL_DELETE( obj1 );
   MEMPOOL_DELETE( obj2 );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)0, pool.getAllocationsCount() );

   // reset the pool
   pool.reset();

   // allocate objects from the start
   obj1 = new ( pool ) MemoryPoolObjectMock( 1 );
   obj2 = new ( pool ) MemoryPoolObjectMock( 2 );

   CPPUNIT_ASSERT_EQUAL( (unsigned int)2, pool.getAllocationsCount() );

   // compare the pointers - the respective objects should receive the same allocation addresses
   long newPtr1 = (long)obj1;
   long newPtr2 = (long)obj2;
   CPPUNIT_ASSERT_EQUAL( ptr1, newPtr1 );
   CPPUNIT_ASSERT_EQUAL( ptr2, newPtr2 );

   // delete the objects
   MEMPOOL_DELETE( obj1 );
   MEMPOOL_DELETE( obj2 );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)0, pool.getAllocationsCount() );
}

///////////////////////////////////////////////////////////////////////////////

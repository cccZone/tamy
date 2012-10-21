#include "core-TestFramework/TestFramework.h"
#include "core/MemoryRouter.h"
#include "core/MemoryUtils.h"
#include "core/Array.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   struct TestClass
   {
      DECLARE_ALLOCATOR( TestClass, AM_DEFAULT );
   };

   template< typename T >
   struct TemplateTestClass
   {
      DECLARE_ALLOCATOR( TemplateTestClass, AM_ALIGNED_16 );

      struct NestedClass
      {
         DECLARE_ALLOCATOR( NestedClass, AM_ALIGNED_16 );
      };
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST( MemoryRouter, defaultMemoryAllocation )
{
   MemoryRouter& router = MemoryRouter::getInstance();
   void* ptr = router.alloc( 12, AM_DEFAULT, &router.m_defaultAllocator );
   
   // all we can test is that the memory is correctly allocated and deallocated
   CPPUNIT_ASSERT( ptr != NULL );

   // cleanup
   router.dealloc( ptr, AM_DEFAULT );
}

///////////////////////////////////////////////////////////////////////////////

TEST( MemoryRouter, alignedMemoryAllocation )
{
   MemoryRouter& router = MemoryRouter::getInstance();
   void* ptr = router.alloc( 12, AM_ALIGNED_16, &router.m_defaultAllocator );

   // all we can test is that the memory is correctly allocated and deallocated
   CPPUNIT_ASSERT( ptr != NULL );

   // and that the returned address is aligned to a 16 byte boundary
   bool isAligned = MemoryUtils::isAddressAligned( ptr, 16 );
   CPPUNIT_ASSERT( isAligned );

   // cleanup
   router.dealloc( ptr, AM_ALIGNED_16 );
}

///////////////////////////////////////////////////////////////////////////////

TEST( MemoryRouter, differentAllocationMethods )
{
   DefaultAllocator defaultAllocator;

   // at the beginning, no memory is allocated
   CPPUNIT_ASSERT_EQUAL( (ulong)0, defaultAllocator.getAllocatedMemorySize() );
   
   MemoryRouter& router = MemoryRouter::getInstance();
   void* ptr = router.alloc( 12, AM_ALIGNED_16, &defaultAllocator );

   // memory was indeed allocated - but since we were allocating an aligned block,
   // a larger block was allocated
   CPPUNIT_ASSERT( ptr != NULL );
   CPPUNIT_ASSERT( (ulong)12 < defaultAllocator.getAllocatedMemorySize() );

   // and that the returned address is aligned to a 16 byte boundary
   bool isAligned = MemoryUtils::isAddressAligned( ptr, 16 );
   CPPUNIT_ASSERT( isAligned );

   // cleanup
   router.dealloc( ptr, AM_ALIGNED_16 );

   // allocated memory was correctly released - the entire amount
   CPPUNIT_ASSERT_EQUAL( (ulong)0, defaultAllocator.getAllocatedMemorySize() );
}

///////////////////////////////////////////////////////////////////////////////

TEST( MemoryRouter, objectsAllocation )
{
   MemoryRouter& router = MemoryRouter::getInstance();
   ulong initialAllocatedMemorySize = router.getAllocatedMemorySize();

   TestClass* obj = new TestClass();
   CPPUNIT_ASSERT( obj != NULL );
   CPPUNIT_ASSERT_EQUAL( (ulong)(sizeof( TestClass ) + sizeof(void*)), router.getAllocatedMemorySize() - initialAllocatedMemorySize );

   delete obj;
   CPPUNIT_ASSERT_EQUAL( (ulong)0, router.getAllocatedMemorySize() - initialAllocatedMemorySize );
}

///////////////////////////////////////////////////////////////////////////////

TEST( MemoryRouter, objectsPlacement )
{
   DefaultAllocator defaultAllocator;
   MemoryRouter& router = MemoryRouter::getInstance();
   ulong initialAllocatedMemorySize = router.getAllocatedMemorySize();

   TestClass* obj = new ( &defaultAllocator ) TestClass();
   CPPUNIT_ASSERT( obj != NULL );

   // no memory was allocated in the pool internally managed by the router
   CPPUNIT_ASSERT_EQUAL( (ulong)0, router.getAllocatedMemorySize() - initialAllocatedMemorySize );

   // entire memory was allocated in the specified external pool
   CPPUNIT_ASSERT_EQUAL( (ulong)(sizeof( TestClass ) + sizeof(void*)), defaultAllocator.getAllocatedMemorySize() );

   delete obj;
   CPPUNIT_ASSERT_EQUAL( (ulong)0, router.getAllocatedMemorySize() - initialAllocatedMemorySize );
   CPPUNIT_ASSERT_EQUAL( (ulong)0, defaultAllocator.getAllocatedMemorySize() );
}

///////////////////////////////////////////////////////////////////////////////

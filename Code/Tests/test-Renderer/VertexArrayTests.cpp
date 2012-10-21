#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\VertexArray.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   struct VertexTypeA
   {
      DECLARE_ALLOCATOR( VertexTypeA, AM_DEFAULT );

      int a;
   };

   // -------------------------------------------------------------------------

   struct VertexTypeB
   {
      DECLARE_ALLOCATOR( VertexTypeB, AM_DEFAULT );

      char a;
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(VertexArray, verticesSizes)
{
   TVertexArray<VertexTypeA> arrA;
   TVertexArray<VertexTypeB> arrB;

   arrA.add(VertexTypeA());
   arrB.add(VertexTypeB());
   arrB.add(VertexTypeB());

   VertexArray* arr = &arrA;
   CPPUNIT_ASSERT_EQUAL((std::size_t)4, arr->getVtxSize());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, arr->size());

   arr = &arrB;
   CPPUNIT_ASSERT_EQUAL((std::size_t)1, arr->getVtxSize());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, arr->size());
}

///////////////////////////////////////////////////////////////////////////////

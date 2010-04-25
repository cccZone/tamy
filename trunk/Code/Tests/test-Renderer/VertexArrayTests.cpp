#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\VertexArray.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   struct VertexTypeA
   {
      static DWORD FVF;
      int a;
   };
   DWORD VertexTypeA::FVF = 1;

   // -------------------------------------------------------------------------

   struct VertexTypeB
   {
      static DWORD FVF;
      char a;
   };
   DWORD VertexTypeB::FVF = 2;

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
   CPPUNIT_ASSERT_EQUAL((DWORD)1, arr->getFVF());
   CPPUNIT_ASSERT_EQUAL((std::size_t)4, arr->getVtxSize());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, arr->size());

   arr = &arrB;
   CPPUNIT_ASSERT_EQUAL((DWORD)2, arr->getFVF());
   CPPUNIT_ASSERT_EQUAL((std::size_t)1, arr->getVtxSize());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, arr->size());
}

///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

#define COMPARE_VEC(v1, v2)                                                   \
   CPPUNIT_ASSERT(fabs(v1.x - v2.x) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.y - v2.y) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.z - v2.z) < 0.0001f);                               \

///////////////////////////////////////////////////////////////////////////////

#define COMPARE_MTX(rhs, lhs)                                                 \
{                                                                             \
   for (char col = 0; col < 4; ++col)                                         \
   {                                                                          \
      for (char row = 0; row < 4; ++row)                                      \
      {                                                                       \
         CPPUNIT_ASSERT(fabs(rhs.m[col][row] - lhs.m[col][row]) < 0.0001f);   \
      }                                                                       \
   }                                                                          \
}

///////////////////////////////////////////////////////////////////////////////

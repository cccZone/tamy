#pragma once

#include <d3dx9.h>

///////////////////////////////////////////////////////////////////////////////

#define COMPARE_EULER(v1, v2)                                                 \
   CPPUNIT_ASSERT(fabs(v1.pitch - v2.pitch) < 0.0001f);                       \
   CPPUNIT_ASSERT(fabs(v1.yaw - v2.yaw) < 0.0001f);                           \
   CPPUNIT_ASSERT(fabs(v1.roll - v2.roll) < 0.0001f);

///////////////////////////////////////////////////////////////////////////////

#define COMPARE_VEC(v1, v2)                                                   \
   CPPUNIT_ASSERT(fabs(v1.x - v2.x) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.y - v2.y) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.z - v2.z) < 0.0001f);

///////////////////////////////////////////////////////////////////////////////

#define COMPARE_VEC4(v1, v2)                                                  \
   CPPUNIT_ASSERT(fabs(v1.x - v2.x) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.y - v2.y) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.z - v2.z) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.w - v2.w) < 0.0001f);

///////////////////////////////////////////////////////////////////////////////

#define COMPARE_QUAT(v1, v2)                                                  \
   CPPUNIT_ASSERT(fabs(v1.x - v2.x) < 0.001f);                                \
   CPPUNIT_ASSERT(fabs(v1.y - v2.y) < 0.001f);                                \
   CPPUNIT_ASSERT(fabs(v1.z - v2.z) < 0.001f);                                \
   CPPUNIT_ASSERT(fabs(v1.w - v2.w) < 0.001f);

///////////////////////////////////////////////////////////////////////////////

#define COMPARE_QUAT_ALLOW_CONJUGATION(v1, v2)                                \
   {                                                                          \
      Quaternion conjV2;                                                      \
      conjV2 = v2; conjV2.conjugate();                                        \
      if ( fabs(v1.x - v2.x) >= 0.001f || fabs(v1.y - v2.y) >= 0.001f || fabs(v1.z - v2.z) >= 0.001f || fabs(v1.w - v2.w) >= 0.001f ) \
      {                                                                       \
         if ( fabs(v1.x + v2.x) >= 0.001f || fabs(v1.y + v2.y) >= 0.001f || fabs(v1.z + v2.z) >= 0.001f || fabs(v1.w + v2.w) >= 0.001f ) \
         {                                                                    \
            if ( fabs(v1.x - conjV2.x) >= 0.001f || fabs(v1.y - conjV2.y) >= 0.001f || fabs(v1.z - conjV2.z) >= 0.001f || fabs(v1.w - conjV2.w) >= 0.001f ) \
            {                                                                 \
               if ( fabs(v1.x + conjV2.x) >= 0.001f || fabs(v1.y + conjV2.y) >= 0.001f || fabs(v1.z + conjV2.z) >= 0.001f || fabs(v1.w + conjV2.w) >= 0.001f ) \
               {                                                              \
                  CPPUNIT_ASSERT( false );                                    \
               }                                                              \
            }                                                                 \
         }                                                                    \
      }                                                                       \
   }
   /*
      CPPUNIT_ASSERT(fabs(v1.w - v2.w) < 0.001f);                             \
      float dx = fabs(v1.x - v2.x);                                           \
      float dy = fabs(v1.y - v2.y);                                           \
      float dz = fabs(v1.z - v2.z);                                           \
      if ( dx > 1e-3 || dy > 1e-3 || dz > 1e-3 )                              \
      {                                                                       \
         dx = fabs( v1.x + v2.x );                                            \
         dy = fabs( v1.y + v2.y );                                            \
         dz = fabs( v1.z + v2.z );                                            \
         CPPUNIT_ASSERT(dx < 0.001f);                                         \
         CPPUNIT_ASSERT(dy < 0.001f);                                         \
         CPPUNIT_ASSERT(dz < 0.001f);                                         \
      }                                                                       \
   }
   */
///////////////////////////////////////////////////////////////////////////////

#define COMPARE_QUAT_AXIS_ANGLE(q, axis, angle)                               \
   {                                                                          \
      float cqaaAngle; Vector cqaaAxis;                                       \
      cqaaAngle = RAD2DEG( q.getAngle() );                                    \
      q.getAxis( cqaaAxis );                                                  \
      CPPUNIT_ASSERT(fabs(cqaaAngle - angle) < 0.0001f);                      \
      COMPARE_VEC( axis, cqaaAxis );                                          \
   }

///////////////////////////////////////////////////////////////////////////////

#define COMPARE_PLANE(v1, v2)                                                 \
   CPPUNIT_ASSERT(fabs(v1.a - v2.a) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.b - v2.b) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.c - v2.c) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.d - v2.d) < 0.0001f);

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

#pragma once

#include <d3dx9.h>

///////////////////////////////////////////////////////////////////////////////

#define COMPARE_EULER(v1, v2)                                                 \
   CPPUNIT_ASSERT(fabs(v1.getPitch() - v2.getPitch()) < 0.0001f);             \
   CPPUNIT_ASSERT(fabs(v1.getYaw() - v2.getYaw()) < 0.0001f);                 \
   CPPUNIT_ASSERT(fabs(v1.getRoll() - v2.getRoll()) < 0.0001f);

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
   if ( fabs(v1[0] - v2[0]) >= 0.001f ||                                        \
        fabs(v1[1] - v2[1]) >= 0.001f ||                                        \
        fabs(v1[2] - v2[2]) >= 0.001f ||                                        \
        fabs(v1[3] - v2[3]) >= 0.001f )                                         \
   {                                                                          \
      CPPUNIT_ASSERT( false );                                                \
   }

///////////////////////////////////////////////////////////////////////////////

#define COMPARE_QUAT_ALLOW_CONJUGATION(v1, v2)                                \
   {                                                                          \
      Quaternion conjV2;                                                      \
      conjV2 = v2; conjV2.conjugate();                                        \
      if ( fabs(v1[0] - v2[0]) >= 0.001f || fabs(v1[1] - v2[1]) >= 0.001f || fabs(v1[2] - v2[2]) >= 0.001f || fabs(v1[3] - v2[3]) >= 0.001f ) \
      {                                                                       \
         if ( fabs(v1[0] + v2[0]) >= 0.001f || fabs(v1[1] + v2[1]) >= 0.001f || fabs(v1[2] + v2[2]) >= 0.001f || fabs(v1[3] + v2[3]) >= 0.001f ) \
         {                                                                    \
            if ( fabs(v1[0] - conjV2[0]) >= 0.001f || fabs(v1[1] - conjV2[1]) >= 0.001f || fabs(v1[2] - conjV2[2]) >= 0.001f || fabs(v1[3] - conjV2[3]) >= 0.001f ) \
            {                                                                 \
               if ( fabs(v1[0] + conjV2[0]) >= 0.001f || fabs(v1[1] + conjV2[1]) >= 0.001f || fabs(v1[2] + conjV2[2]) >= 0.001f || fabs(v1[3] + conjV2[3]) >= 0.001f ) \
               {                                                              \
                  CPPUNIT_ASSERT( false );                                    \
               }                                                              \
            }                                                                 \
         }                                                                    \
      }                                                                       \
   }

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
   CPPUNIT_ASSERT(fabs(v1.a - v2[0]) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.b - v2[1]) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.c - v2[2]) < 0.0001f);                               \
   CPPUNIT_ASSERT(fabs(v1.d - v2[3]) < 0.0001f);

///////////////////////////////////////////////////////////////////////////////

#define COMPARE_MTX(rhs, lhs)                                                 \
{                                                                             \
   bool err = false;                                                          \
   for (char col = 0; col < 4 && !err ; ++col)                                \
   {                                                                          \
      for (char row = 0; row < 4 && !err; ++row)                              \
      {                                                                       \
         if ( fabs(rhs.m[col][row] - lhs.m[col][row]) >= 0.0001f )            \
         {                                                                    \
            CPPUNIT_ASSERT( false );                                          \
            err = true;                                                       \
            break;                                                            \
         }                                                                    \
      }                                                                       \
   }                                                                          \
}

///////////////////////////////////////////////////////////////////////////////

/// @file   core/MemoryUtils.h
/// @brief  memory management utility functions
#ifndef _MEMORY_UTILS_H
#define _MEMORY_UTILS_H

#include "core\types.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Memory management utility functions.
 */
class MemoryUtils
{
public:
   static int        ALIGNMENT;

public:
   /**
    * Allocates an aligned block of memory of the specified size.
    *
    * The memory is allocated on the system heap.
    *
    * @param size
    * @param alignment
    */
   static void* mallocAligned( uint size, uint alignment );

   /**
    * Frees memory allocated with 'mallocAligned' method.
    *
    * @param ptr
    */
   static void freeAligned( void* ptr );


   /**
    * Aligns a pointer using the specified alignment.
    *
    * @param ptr           original, unaligned pointer
    * @param alignment     alignment boundary in bytes
    * @return              aligned pointer
    */
   static void* alignPointer( void* ptr, uint alignment );

   /**
    * Dis-aligns a pointer aligned with 'alignPointer' method.
    *
    * @param   alignedPtr
    * @return  original, unaligned pointer
    */
   static void* resolveAlignedPointer( void* alignedPtr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro adds a custom memory allocator that will align a class to the specified address boundary.
 */
#define ALIGNED_STRUCT() \
public: \
   static void* operator new( size_t size ) \
   { \
      void* ptr = MemoryUtils::mallocAligned( size, MemoryUtils::ALIGNMENT ); \
      return ptr; \
   } \
   \
   static void operator delete( void* p ) \
   { \
      MemoryUtils::freeAligned( p ); \
   } \
   static void* operator new[]( size_t size ) \
   { \
      void* ptr = MemoryUtils::mallocAligned( size, MemoryUtils::ALIGNMENT ); \
      return ptr; \
   } \
      \
      static void operator delete[]( void* p ) \
   { \
      MemoryUtils::freeAligned( p ); \
   }

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro adds a custom memory allocator that will align a class to the specified address boundary.
 */
#define ALIGNED_CLASS() \
   ALIGNED_STRUCT() \
private:


#endif // _MEMORY_UTILS_H

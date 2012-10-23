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
   /**
    * Aligns a pointer using the specified alignment.
    *
    * @param ptr           original, unaligned pointer
    * @param alignment     alignment boundary in bytes
    * @return              aligned pointer
    */
   static void* alignAddress( void* ptr, uint alignment );

   /**
    * Aligns a pointer using the specified alignment and stores the original address
    * 4 bytes before the returned pointer.
    *
    * @param ptr           original, unaligned pointer
    * @param alignment     alignment boundary in bytes
    * @return              aligned pointer
    */
   static void* alignAddressAndStoreOriginal( void* ptr, uint alignment );

   /**
    * Reverse-aligns a pointer aligned with 'alignAddressAndStoreOriginal' method.
    *
    * @param   alignedPtr
    * @return  original, unaligned pointer
    */
   static void* resolveAlignedAddress( void* alignedPtr );

   /**
    * Checks if the specified address is aligned to the specified boundary.
    *
    * @param ptr
    * @param alignment
    */
   static bool isAddressAligned( void* ptr, uint alignment );

   /**
    * Calculates the amount of memory needed to store an aligned version of an object with the specified size
    * using the specified alignment.
    *
    * @param size
    * @param alignment
    */
   static size_t calcAlignedSize( size_t size, uint alignment );
};

///////////////////////////////////////////////////////////////////////////////

#endif // _MEMORY_UTILS_H

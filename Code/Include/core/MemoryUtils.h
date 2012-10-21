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
   static void* alignPointer( void* ptr, uint alignment );

   /**
    * Dis-aligns a pointer aligned with 'alignPointer' method.
    *
    * @param   alignedPtr
    * @return  original, unaligned pointer
    */
   static void* resolveAlignedPointer( void* alignedPtr );

   /**
    * Checks if the specified address is aligned to the specified boundary.
    *
    * @param ptr
    * @param alignment
    */
   static bool isAddressAligned( void* ptr, uint alignment );
};

///////////////////////////////////////////////////////////////////////////////

#endif // _MEMORY_UTILS_H

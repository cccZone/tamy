/// @file   core/DefaultAllocator.h
/// @brief  default memory allocator
#pragma once


///////////////////////////////////////////////////////////////////////////////

/**
 * Default memory allocator.
 */
class DefaultAllocator
{
public:
   /**
    * Destroys an object allocated by the allocator
    */
   template< typename T >
   void destroy( T* obj )
   {
      if ( obj )
      {
         obj->~T();
         dealloc( obj );
      }
   }

   /**
    * Allocates a chunk of memory of the specified size.
    *
    * @param size
    */
   void* alloc( size_t size );

   /**
    * Deallocates the memory from the specified address.
    */
   void dealloc( void* ptr );
};

///////////////////////////////////////////////////////////////////////////////

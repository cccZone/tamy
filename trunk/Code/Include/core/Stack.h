/// @file   core\Stack
/// @brief  simple stack implementation
#ifndef _STACK_H
#define _STACK_H

#include <stdio.h>
#include "core/DefaultAllocator.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is an extremely simple implementation of a stack.
 */
template< typename T, typename TAllocator = DefaultAllocator >
struct Stack
{
private:
   struct StackSeg
   {
      StackSeg*   m_prev;
      T           m_elem;

      /**
       * Constructor.
       *
       * @param prev
       * @param elem
       */
      StackSeg( StackSeg* prev, T elem );
      ~StackSeg();

      // ----------------------------------------------------------------------
      // Placement allocation operators
      // ----------------------------------------------------------------------
      void* operator new( size_t size, TAllocator* allocator );
      void operator delete( void* ptr, TAllocator* allocator );

   private:
      // ----------------------------------------------------------------------
      // Disabled regular allocation operators
      // ----------------------------------------------------------------------
      void* operator new( size_t size );
      void operator delete( void* ptr );
   };

private:
   StackSeg*         m_top;

   TAllocator*       m_defaultAllocator;
   TAllocator*       m_allocator;

public:
   /**
    * Constructor.
    *
    * @param allocator     custom memory allocator
    */
   Stack( TAllocator* allocator = NULL );
   ~Stack();

   /**
    * Pushes a new element on top of the stack
    */
   void push( T elem );

   /**
    * Pops an element from the top of the stack
    */
   T pop();

   /**
    * Allows to peek at the element that's at the top of the stack ( const version )
    */
   const T& top() const;

   /**
    * Allows to peek at the element that's at the top of the stack ( non-const version )
    */
   T& top();

   /**
    * Tells if the stack is empty.
    */
   bool empty() const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core/Stack.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _STACK_H

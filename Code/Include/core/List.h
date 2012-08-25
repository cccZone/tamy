/// @file   core/List.h
/// @brief  double-linked list implementation
#ifndef _LIST_H
#define _LIST_H

#include "core/DefaultAllocator.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Double-linked list implementation.
 */
template< typename T, typename TAllocator = DefaultAllocator >
class List
{
private:
   struct Elem
   {
      T        m_element;
      Elem*    m_nextElem;
      Elem*    m_prevElem;

      Elem( T element ) : m_element( element ), m_nextElem( NULL ), m_prevElem( NULL ) {}
      ~Elem() { m_nextElem = NULL; m_prevElem = NULL; }

      // ----------------------------------------------------------------------
      // Placement allocation operators
      // ----------------------------------------------------------------------
      void* operator new( size_t size, TAllocator& allocator );
      void operator delete( void* ptr, TAllocator& allocator );

   private:
      // ----------------------------------------------------------------------
      // Disabled regular allocation operators
      // ----------------------------------------------------------------------
      void* operator new( size_t size );
      void operator delete( void* ptr );
   };

   TAllocator*    m_defaultAllocator;
   TAllocator*    m_allocator;

   Elem*          m_head;
   Elem*          m_tail;

public:
   /**
    * Constructor.
    *
    * @param allocator     memory allocator to use with the list
    */
   List( TAllocator* allocator = NULL );
   ~List();

   /**
    * Checks if the list stores no elements.
    */
   bool empty() const;

   /**
    * Appends an element at the back of the list.
    *
    * @param elem
    */
   void pushBack( T elem );

   /**
    * Removes the last element on the list.
    */
   void popBack();

   /**
    * Inserts an element at the front of the list.
    *
    * @param elem
    */
   void pushFront( T elem );

   /**
    * Removes the first element on the list.
    */
   void popFront();

   /**
    * Accesses the last element on the list ( const version ).
    */
   T back() const;

   /**
    * Accesses the last element on the list ( non-const version ).
    */
   T back();

   /**
    * Accesses the first element on the list ( const version ).
    */
   T front() const;

   /**
    * Accesses the first element on the list ( non-const version ).
    */
   T front();
};

///////////////////////////////////////////////////////////////////////////////

#include "core/List.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _LIST_H

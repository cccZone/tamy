#ifndef _LIST_H
#error "This file can only be included from List.h"
#else

#include "core/MemoryRouter.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
List< T, TAllocator >::List( TAllocator* allocator )
   : m_defaultAllocator( NULL )
   , m_allocator( allocator )
   , m_head( NULL )
   , m_tail( NULL )
{
   if ( !m_allocator )
   {
      m_defaultAllocator = new TAllocator();
      m_allocator = m_defaultAllocator;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
List< T, TAllocator >::~List()
{
   Elem* toRemove;
   for( Elem* elem = m_head; elem != NULL;  )
   {
      toRemove = elem;
      elem = elem->m_nextElem;
      delete toRemove;
   }

   m_tail = NULL;

   delete m_defaultAllocator;
   m_defaultAllocator = NULL;
   m_allocator = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
bool List< T, TAllocator >::empty() const
{
   return m_head == NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void List< T, TAllocator >::pushBack( T elem )
{
   Elem* newTail = new ( m_allocator ) Elem( elem );
   if ( m_tail )
   {
      m_tail->m_nextElem = newTail;
      newTail->m_prevElem = m_tail;
      m_tail = newTail;
   }
   else
   {
      m_tail = newTail;
      m_head = newTail;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void List< T, TAllocator >::popBack()
{
   if ( m_tail )
   {
      Elem* removedTail = m_tail;
      m_tail = m_tail->m_prevElem;
      delete removedTail;

      if ( m_tail )
      {
         m_tail->m_nextElem = NULL;
      }
      else
      {
         m_head = NULL;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void List< T, TAllocator >::pushFront( T elem )
{
   Elem* newHead = new ( m_allocator ) Elem( elem );
   if ( m_head )
   {
      m_head->m_prevElem = newHead;
      newHead->m_nextElem = m_head;
      m_head = newHead;
   }
   else
   {
      m_tail = newHead;
      m_head = newHead;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void List< T, TAllocator >::popFront()
{
   if ( m_head )
   {
      Elem* removedHead = m_head;
      m_head = m_head->m_nextElem;
      delete removedHead;

      if ( m_head )
      {
         m_head->m_prevElem = NULL;
      }
      else
      {
         m_tail = NULL;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
T List< T, TAllocator >::back() const
{
   if ( m_tail )
   {
      return m_tail->m_element;
   }
   else
   {
      return 0;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
T List< T, TAllocator >::back()
{
   if ( m_tail )
   {
      return m_tail->m_element;
   }
   else
   {
      return 0;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
T List< T, TAllocator >::front() const
{
   if ( m_head )
   {
      return m_head->m_element;
   }
   else
   {
      return 0;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
T List< T, TAllocator >::front()
{
   if ( m_head )
   {
      return m_head->m_element;
   }
   else
   {
      return 0;
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _LIST_H

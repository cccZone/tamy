#pragma once

/// @file   core\Stack
/// @brief  simple stack implementation

#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This is an extremely simple implementation of a stack.
 */
template<typename T>
struct Stack
{
private:
   template<typename T>
   struct StackSeg
   {
      T elem;
      StackSeg* prev;

      StackSeg() : elem(NULL), prev(NULL) {}

      StackSeg(const StackSeg& rhs) 
      : elem(rhs.elem)
      , prev(new StackSeg(*(rhs.prev))) 
      {}

      ~StackSeg() {delete prev; prev = NULL; elem = NULL;}
   };

private:
   StackSeg<T>*   m_top;

public:
   /**
    * Default constructor.
    */
   Stack() : m_top( NULL ) {}

   /**
    * Copy constructor.
    */
   Stack( const Stack& rhs ) : m_top( new StackSeg< T >( *rhs.m_top ) ) {}
   ~Stack() { delete m_top; m_top = NULL; }

   /**
    * Pushes a new element on top of the stack
    */
   void push( T elem )
   {
      StackSeg<T>* newSeg = new StackSeg<T>();
      newSeg->prev = m_top;
      newSeg->elem = elem;

      m_top = newSeg;
   }

   /**
    * Pops an element from the top of the stack
    */
   T pop()
   {
      StackSeg<T>* removedSeg = m_top;
      m_top = removedSeg->prev;
      removedSeg->prev = NULL;
      T elem = removedSeg->elem;
      delete removedSeg;

      return elem;
   }

   /**
    * Allows to peek at the element that's at the top of the stack ( const version )
    */
   const T& top() const { return m_top->elem; }

   /**
    * Allows to peek at the element that's at the top of the stack ( non-const version )
    */
   T& top() { return m_top->elem; }

   /**
    * Tells if the stack is empty.
    */
   bool empty() const {return m_top == NULL;}
};

///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This is an extremally simple implementation of a stack
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
      ~StackSeg() {delete prev; prev = NULL; elem = NULL;}
   };

private:
   StackSeg<T>* top;

public:
   Stack() : top(NULL) {}
   ~Stack() {delete top; top = NULL;}

   void push(T elem)
   {
      StackSeg<T>* newSeg = new StackSeg<T>();
      newSeg->prev = top;
      newSeg->elem = elem;

      top = newSeg;
   }

   T pop()
   {
      StackSeg<T>* removedSeg = top;
      top = removedSeg->prev;
      removedSeg->prev = NULL;
      T elem = removedSeg->elem;
      delete removedSeg;

      return elem;
   }

   bool empty() const {return top == NULL;}
};

///////////////////////////////////////////////////////////////////////////////

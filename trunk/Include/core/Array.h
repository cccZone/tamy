#pragma once

#include "core\Assert.h"
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////

/** 
 * A marker used to indicate that the element is not there in the array
 */

#define EOA (unsigned int)-1

///////////////////////////////////////////////////////////////////////////////

template<typename T>
class Array
{
private:
   unsigned int m_size;
   unsigned int m_elementsCount;
   T* m_arr;

public:
   /**
    * The parameter passed here preallocates the amount of memory necessary
    * for the array to store the passed number of elements.
    * It WILL NOT allocate these elements !!!
    */
   Array(unsigned int size = 1)
      : m_size(1),
      m_elementsCount(0)
   {
      m_arr = new T[1];

      if (size > 1)
      {
         resize(size);
      }
   }

   Array(const Array& rhs)
      : m_size(rhs.m_size),
      m_elementsCount(rhs.m_elementsCount),
      m_arr(new T[m_size])
   {
      for (unsigned int i = 0; i < m_elementsCount; ++i)
      {
         m_arr[i] = rhs.m_arr[i];
      }
   }

   ~Array()
   {
      delete [] m_arr;
      m_arr = NULL;

      m_elementsCount = 0;
      m_size = 0;
   }

   /**
    * Clears the contents of the array
    */
   void clear()
   {
      m_elementsCount = 0;
   }

   /**
    * The method returns the number of elements that can be put
    * in the array without it being resized
    */
   unsigned int containerSize() const {return m_size;}

   /**
    * The method returns the number of elements in the array
    */
   unsigned int size() const {return m_elementsCount;}

   /**
    * This method copies the contents of the array passed in as a parameter
    * and appends it to the back of its own array
    */
   void copyFrom(const Array<T>& rhs)
   {
      resize(size() + rhs.size());
      for (unsigned int i = 0; i < rhs.m_elementsCount; ++i)
      {
         m_arr[m_elementsCount + i] = rhs.m_arr[i];
      }
      m_elementsCount += rhs.m_elementsCount;
   }

   /**
    * The method resizes the array, preparing it to accumulate
    * a number of elements without a need to resize in the process.
    *
    * CAUTION: An array can't be downsized
    */
   void resize(unsigned int newSize)
   {
      if (newSize < m_size) {return;}

      unsigned int newSizePow2 = m_size;
      while (newSizePow2 < newSize)
      {
         newSizePow2 = newSizePow2 << 1;
      }

      T* newArr = new T[newSizePow2];
         
      for (unsigned int i = 0; i < m_elementsCount; ++i)
      {
         newArr[i] = m_arr[i];
      }
      delete m_arr;
      m_arr = newArr;
      m_size = newSizePow2;
   }

   /**
    * The method appends a new element at the end of the array, 
    * resizing it if necessary
    */
   void push_back(const T& elem)
   {
      unsigned int newElementsCount = m_elementsCount + 1;
      if (newElementsCount > m_size)
      {
         resize(m_size << 1);
      }

      m_arr[m_elementsCount] = elem;
      m_elementsCount = newElementsCount;
   }

   /**
    * The method removes an element at the given index
    */
   void remove(unsigned int idx)
   {
      ASSERT(idx < m_elementsCount, "index out of array boundaries"); 

      for (unsigned int i = idx + 1; i < m_elementsCount; ++i)
      {
         m_arr[i - 1] = m_arr[i];
      }
      m_elementsCount--;
   }

   /**
    * The method allows to access the given element of the array
    */
   T& at(unsigned int idx)
   {
      ASSERT(idx < m_elementsCount,  "index out of array boundaries"); 
      return m_arr[idx];
   }

   /**
    * The method allows to access the given element of the array,
    * making sure it doesn't get modified though (read-only access)
    */
   const T& at(unsigned int idx) const
   {
      ASSERT(idx < m_elementsCount, "index out of array boundaries"); 
      return m_arr[idx];
   }

   /**
    * The method locates an element in the array (using the operator==)
    * and returns the position it's on (0-based).
    *
    * EOA is returned if the element is not found.
    *
    * The method allows to start the search from an arbitrary index
    * in order to allow the user to look for multiple elements having
    * the same value
    */
   unsigned int find(const T& elem, unsigned int startPos = 0)
   {
      if (startPos >= m_elementsCount)
      {
         return EOA;
      }

      for (unsigned int i = startPos; i < m_elementsCount; ++i)
      {
         if (m_arr[i] == elem)
         {
            return i;
         }
      }

      return EOA;
   }

   /**
    * cast operator that allows to use the container as a plain vanilla c++ standard array
    */
   operator T*()
   {
      return m_arr;
   }

   /**
    * cast operator that allows to use the container as a plain vanilla c++ standard array
    * (in a read-only way)
    */
   operator const T*() const
   {
      return m_arr;
   }
};

///////////////////////////////////////////////////////////////////////////////

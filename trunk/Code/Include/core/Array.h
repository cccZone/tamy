#ifndef _ARRAY_H
#define _ARRAY_H

/// @file   core\Array.h
/// @brief  a reusable array container


#include "core\Assert.h"
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////

/** 
 * A marker used to indicate that the element is not there in the array.
 */

#define EOA (unsigned int)-1

///////////////////////////////////////////////////////////////////////////////

/**
 * A plain c-style array turned into object oriented gizmo - has the speed
 * of c-style array, and the powers of object oriented containers.
 */
template< typename T >
class Array
{
private:
   unsigned int m_size;
   unsigned int m_elementsCount;
   T* m_arr;

public:
   /**
    * Constructor.
    *
    *The parameter passed here preallocates the amount of memory necessary
    * for the array to store the passed number of elements.
    * It WILL NOT allocate these elements !!!
    *
    * @param size    initial array size (array will allocate memory to
    *                incorporate at least that many elements up front).
    */
   Array(unsigned int size = 1);

   Array(const Array& rhs);

   ~Array();

   /**
    * Clears the contents of the array.
    */
   void clear();

   /**
    * The method returns the number of elements that can be put
    * in the array without it being resized.
    */
   unsigned int containerSize() const;

   /**
    * The method returns the number of elements in the array.
    */
   unsigned int size() const;

   /**
    * This method copies the contents of the array passed in as a parameter
    * and appends it to the back of its own array.
    *
    * @param rhs  array from which we want to copy elements
    */
   void copyFrom(const Array<T>& rhs);

   /**
    * The method reallocates th memory the array is using, preparing it 
    * for accumulation of a number of elements without a need to resize 
    * in the process.
    *
    * CAUTION: An array can't be downsized
    *
    * @param newSize    how many elements we want the array to allocate
    *                   memory for.
    */
   void allocate(unsigned int newSize);

   /**
    * The method resizes the array, creating new elements
    * as necessary.
    *
    * CAUTION: An array can't be downsized - the number of
    * reserved elems will remain the same - however the overflow
    * of the elements will be released.
    *
    * @param newSize       how many elements we want to have in the array
    *                      after this method completes
    * @param defaultValue  value for the automatically created
    *                      elements
    */
   void resize(unsigned int newSize, const T& defaultValue = 0 );

   /**
    * The method appends a new element at the end of the array, 
    * resizing it if necessary.
    *
    * @param elem    elements that's gonna be appended
    */
   void push_back(const T& elem);

   /**
    * Accesses the last element in the array (providing there is one).
    * (const version)
    */
   const T& back() const;

   /**
    * Accesses the last element in the array (providing there is one).
    */
   T& back();

   /**
    * The method removes an element at the given index.
    *
    * @param idx     index of an element that's gonna be removed
    */
   void remove(unsigned int idx);

   /**
    * The method allows to access the given element of the array.
    */
   T& at(unsigned int idx);

   /**
    * The method allows to access the given element of the array,
    * making sure it doesn't get modified though (read-only access)
    */
   const T& at(unsigned int idx) const;

   /**
    * The method locates an element in the array (using the operator==)
    * and returns the position it's on (0-based).
    *
    * EOA is returned if the element is not found.
    *
    * The method allows to start the search from an arbitrary index
    * in order to allow the user to look for multiple elements having
    * the same value.
    */
   unsigned int find(const T& elem, unsigned int startPos = 0) const;

   /**
    * Cast operator that allows to use the container as a plain vanilla 
    * c++ standard array.
    */
   operator T*();

   /**
    * Cast operator that allows to use the container as a plain vanilla 
    * c++ standard array. (in a read-only way)
    */
   operator const T*() const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core\Array.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _ARRAY_H

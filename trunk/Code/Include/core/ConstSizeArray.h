#ifndef _FAST_RELEASE_ARRAY
#define _FAST_RELEASE_ARRAY

/// @file   core\ConstSizeArray.h
/// @brief  a speedy array for often object releases

#include "core\Array.h"
#include "core\Stack.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A plain c-style array turned into object oriented gizmo - has the speed
 * of c-style array, and the powers of object oriented containers.
 */
template<typename T>
class ConstSizeArray
{
public:
   class iterator
   {
   private:
      const Array<T>* m_elements;
      const Array<bool>* m_isFree;
      unsigned int m_idx;

   public:
      iterator(const iterator& rhs);

      bool operator==(const iterator& rhs) const;
      bool operator!=(const iterator& rhs) const;

      iterator& operator=(const iterator& rhs);

      // prefix
      iterator& operator++();
      iterator& operator--();

      // postfix
      iterator operator++(int);
      iterator operator--(int);

      const T& operator*() const;

   private:
      iterator(const Array<T>& elements,
               const Array<bool>& isFree,
               unsigned int idx);

      unsigned int getNext() const;
      unsigned int getPrev() const;

      friend class ConstSizeArray;
   };

private:
   Array<T>* m_elements;
   Array<bool>* m_isFree;
   Stack<unsigned int>* m_freePos;

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
   ConstSizeArray(unsigned int size = 1);

   ConstSizeArray(const ConstSizeArray& rhs);

   ~ConstSizeArray();

   /**
    * Clears the contents of the array.
    */
   void clear();

   /**
    * The method returns the number of elements in the array.
    */
   unsigned int size() const;

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
   void resize(unsigned int newSize, const T& defaultValue = 0);

   /**
    * The method inserts a new element to the array, resizing it if necessary.
    *
    * @param elem    elements that's gonna be appended
    * @return        array index at which it was inserted
    */
   unsigned int insert(const T& elem);

   /**
    * The method removes an element at the given index.
    *
    * @param idx     index of an element that's gonna be removed
    */
   void remove(unsigned int idx);

   /**
    * The method gives access to the specified element of the array.
    */
   T& at(unsigned int idx);

   /**
    * The method gives access to the specified element of the array,
    * making sure it doesn't get modified though (read-only access)
    */
   const T& at(unsigned int idx) const;

   /**
    * The operator gives access to the specified element of the array.
    */
   T& operator[](unsigned int idx);

   /**
    * The operator gives access to the specified element of the array,
    * making sure it doesn't get modified though (read-only access)
    */
   const T& operator[](unsigned int idx) const;

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
    * Creates an iterator set at the beginning of the collection.
    *
    * @return  array iterator
    */
   iterator begin() const;

   /**
    * Creates an iterator set at the end of the collection.
    *
    * @return  array iterator
    */
   iterator end() const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ConstSizeArray.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _FAST_RELEASE_ARRAY_H

#ifndef _ARRAY_H
#error "This file can only be included from Array.h"
#else


///////////////////////////////////////////////////////////////////////////////

template<typename T>
Array<T>::Array(unsigned int size = 1)
: m_size(1),
m_elementsCount(0)
{
   m_arr = new T[1];

   if (size > 1)
   {
      allocate(size);
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
Array<T>::Array(const Array& rhs)
: m_size(rhs.m_size),
m_elementsCount(rhs.m_elementsCount),
m_arr(new T[m_size])
{
   for (unsigned int i = 0; i < m_elementsCount; ++i)
   {
      m_arr[i] = rhs.m_arr[i];
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
Array<T>::~Array()
{
   delete [] m_arr;
   m_arr = NULL;

   m_elementsCount = 0;
   m_size = 0;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
void Array<T>::clear()
{
   m_elementsCount = 0;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
unsigned int Array<T>::containerSize() const 
{
   return m_size;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
unsigned int Array<T>::size() const 
{
   return m_elementsCount;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
void Array<T>::copyFrom(const Array<T>& rhs)
{
   allocate(size() + rhs.size());
   for (unsigned int i = 0; i < rhs.m_elementsCount; ++i)
   {
      m_arr[m_elementsCount + i] = rhs.m_arr[i];
   }
   m_elementsCount += rhs.m_elementsCount;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
void Array<T>::allocate(unsigned int newSize)
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

///////////////////////////////////////////////////////////////////////////////

template<typename T>
void Array<T>::resize(unsigned int newSize, const T& defaultValue = 0)
{
   allocate(newSize);

   if (newSize < m_elementsCount)
   {
      for (unsigned int i = newSize; i < m_elementsCount; ++i)
      {
         m_arr[i] = defaultValue;
      }
   }
   else
   {
      for (unsigned int i = m_elementsCount; i < newSize; ++i)
      {
         m_arr[i] = defaultValue;
      }
   }
   m_elementsCount = newSize;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
void Array<T>::push_back(const T& elem)
{
   unsigned int newElementsCount = m_elementsCount + 1;
   if (newElementsCount > m_size)
   {
      allocate(m_size << 1);
   }

   m_arr[m_elementsCount] = elem;
   m_elementsCount = newElementsCount;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
const T& Array<T>::back() const
{
   ASSERT(m_elementsCount > 0,  "array is empty"); 
   return m_arr[m_elementsCount - 1];
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
T& Array<T>::back()
{
   ASSERT(m_elementsCount > 0,  "array is empty"); 
   return m_arr[m_elementsCount - 1];
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
void Array<T>::remove(unsigned int idx)
{
   ASSERT(idx < m_elementsCount, "index out of array boundaries"); 

   for (unsigned int i = idx + 1; i < m_elementsCount; ++i)
   {
      m_arr[i - 1] = m_arr[i];
   }
   m_elementsCount--;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
T& Array<T>::at(unsigned int idx)
{
   ASSERT(idx < m_elementsCount,  "index out of array boundaries"); 
   return m_arr[idx];
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
const T& Array<T>::at(unsigned int idx) const
{
   ASSERT(idx < m_elementsCount, "index out of array boundaries"); 
   return m_arr[idx];
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
unsigned int Array<T>::find(const T& elem, unsigned int startPos = 0) const
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

///////////////////////////////////////////////////////////////////////////////

template<typename T>
Array<T>::operator T*()
{
   return m_arr;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
Array<T>::operator const T*() const
{
   return m_arr;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _ARRAY_H

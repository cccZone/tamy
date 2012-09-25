#ifndef _ARRAY_H
#error "This file can only be included from Array.h"
#else


///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
Array< T, TAllocator >::Array( unsigned int size, TAllocator* allocator )
   : m_size(1)
   , m_elementsCount(0)
   , m_defaultAllocator( NULL )
   , m_allocator( allocator )
{
   if ( !m_allocator )
   {
      // no allocator was specified, so we need to create our own
      m_defaultAllocator = new TAllocator();
      m_allocator = m_defaultAllocator;
   }

   m_arr = (T*)m_allocator->alloc( sizeof( T ) );

   if ( size > 1 )
   {
      allocate( size );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
Array< T, TAllocator >::Array( const Array& rhs )
   : m_size(rhs.m_size)
   , m_elementsCount( rhs.m_elementsCount )
{
   if ( rhs.m_allocator == rhs.m_defaultAllocator )
   {
      // the other array uses a default allocator, so let's create our own then
      m_defaultAllocator = new TAllocator();
      m_allocator = m_defaultAllocator;
   }
   else
   {
      // use the same allocator as the other array
      m_defaultAllocator = NULL;
      m_allocator = rhs.m_allocator;
   }

   m_arr = (T*)m_allocator->alloc( sizeof( T ) * m_size );

   for ( unsigned int i = 0; i < m_elementsCount; ++i )
   {
      m_arr[i] = rhs.m_arr[i];
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
Array< T, TAllocator >::~Array()
{
   m_allocator->dealloc( m_arr );
   m_arr = NULL;

   m_elementsCount = 0;
   m_size = 0;

   delete m_defaultAllocator;
   m_defaultAllocator = NULL;

   m_allocator = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void Array< T, TAllocator >::clear()
{
   m_elementsCount = 0;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
unsigned int Array< T, TAllocator >::containerSize() const 
{
   return m_size;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
bool Array< T, TAllocator >::empty() const 
{
   return m_elementsCount == 0;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
unsigned int Array< T, TAllocator >::size() const 
{
   return m_elementsCount;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void Array< T, TAllocator >::copyFrom( const Array<T>& rhs )
{
   allocate( size() + rhs.size() );
   for ( unsigned int i = 0; i < rhs.m_elementsCount; ++i )
   {
      m_arr[m_elementsCount + i] = rhs.m_arr[i];
   }
   m_elementsCount += rhs.m_elementsCount;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void Array< T, TAllocator >::allocate(unsigned int newSize)
{
   if (newSize < m_size) {return;}

   unsigned int newSizePow2 = m_size;
   while (newSizePow2 < newSize)
   {
      newSizePow2 = newSizePow2 << 1;
   }

   T* newArr = (T*)m_allocator->alloc( sizeof( T ) * newSizePow2 );
   for (unsigned int i = 0; i < m_elementsCount; ++i)
   {
      newArr[i] = m_arr[i];
   }
   m_allocator->dealloc( m_arr );
   m_arr = newArr;
   m_size = newSizePow2;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void Array< T, TAllocator >::resize(unsigned int newSize, const T& defaultValue )
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

template< typename T, typename TAllocator >
void Array< T, TAllocator >::push_back(const T& elem)
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

template< typename T, typename TAllocator >
const T& Array< T, TAllocator >::back() const
{
   ASSERT_MSG(m_elementsCount > 0,  "array is empty"); 
   return m_arr[m_elementsCount - 1];
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
T& Array< T, TAllocator >::back()
{
   ASSERT_MSG(m_elementsCount > 0,  "array is empty"); 
   return m_arr[m_elementsCount - 1];
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void Array< T, TAllocator >::remove(unsigned int idx)
{
   ASSERT_MSG(idx < m_elementsCount, "index out of array boundaries"); 

   for (unsigned int i = idx + 1; i < m_elementsCount; ++i)
   {
      m_arr[i - 1] = m_arr[i];
   }
   m_elementsCount--;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
T& Array< T, TAllocator >::at(unsigned int idx)
{
   ASSERT_MSG(idx < m_elementsCount,  "index out of array boundaries"); 
   return m_arr[idx];
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
const T& Array< T, TAllocator >::at(unsigned int idx) const
{
   ASSERT_MSG(idx < m_elementsCount, "index out of array boundaries"); 
   return m_arr[idx];
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
unsigned int Array< T, TAllocator >::find(const T& elem, unsigned int startPos = 0) const
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

template< typename T, typename TAllocator >
Array< T, TAllocator >::operator T*()
{
   return m_arr;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
Array< T, TAllocator >::operator const T*() const
{
   return m_arr;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _ARRAY_H

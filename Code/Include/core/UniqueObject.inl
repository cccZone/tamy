#ifndef _UNIQUE_OBJECT_H
#error "This file can only be included from UniqueObject.h"
#else


///////////////////////////////////////////////////////////////////////////////

template< typename T >
unsigned int UniqueObject< T >::m_nextIndex = 0;

///////////////////////////////////////////////////////////////////////////////

template< typename T >
UniqueObject< T >::UniqueObject()
: m_index(m_nextIndex)
{
   ++m_nextIndex;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
UniqueObject< T >::UniqueObject( const UniqueObject< T >& rhs )
: m_index( m_nextIndex )
{
   ++m_nextIndex;
}

///////////////////////////////////////////////////////////////////////////////

#endif _UNIQUE_OBJECT_H

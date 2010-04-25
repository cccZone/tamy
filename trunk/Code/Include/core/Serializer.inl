#ifndef _SERIALIZER_H
#error "This file can only be included from Serializer.h"
#else

/// @file   core\ExternalTypesSerialization.h
/// @brief  serialization methods for the external data types

#include <vector>
#include "core\Serializer.h"
#include "core\SerializerImpl.h"

///////////////////////////////////////////////////////////////////////////////

template< typename Derived >
Derived* Loader::load()
{
   // recover the dependencies
   m_dependencies.clear();
   m_dependencies.push_back( NULL );

   // load the dependencies
   unsigned int count = 0;
   m_impl->read( ( byte* )&count, sizeof( unsigned int ) );

   for ( unsigned int i = 0; i < count; ++i )
   {
      Serializable* ptr = Serializable::load<Serializable>(*this);
      m_dependencies.push_back( ptr );
   }

   // the root object is always the second one in the dependencies map
   // ('cause the first one is the NULL pointer)
   Serializable* root = m_dependencies[1];

   // update the dependencies between the objects
   DependenciesMapper mapper( m_dependencies, PSS_UPDATE );
   count = m_dependencies.size();
   for ( unsigned int i = 1; i < count; ++i )
   {
      m_dependencies[ i ]->save( mapper );
   }

   Derived* derRoot = dynamic_cast< Derived* >( root );
   return derRoot;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
void Serializer::operator<<(std::vector<T>& vec)
{
   PointerSaveState state = getState();
   if ( state == PSS_SAVE )
   {
      unsigned int count = vec.size();
      *this << count;
      for (unsigned int i = 0; i < count; ++i)
      {
         *this << vec[i];
      }
   }
   else if ( state == PSS_LOAD )
   {
      unsigned int count = 0;
      *this << count;
      for (unsigned int i = 0; i < count; ++i)
      {
         vec.push_back(0);
         *this << vec.back();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
void Serializer::operator<<(std::vector<T*>& vec)
{
   PointerSaveState state = getState();
   if ( state == PSS_SAVE )
   {
      unsigned int count = vec.size();
      *this << count;
      for (unsigned int i = 0; i < count; ++i)
      {
         Serializable* obj = dynamic_cast<Serializable*>(vec[i]);
         *this << &obj;
      }
   }
   else if ( state == PSS_LOAD )
   {
      unsigned int count = 0;
      *this << count;
      for (unsigned int i = 0; i < count; ++i)
      {
         Serializable* obj = NULL;
         *this << &obj;

         T* tObj = reinterpret_cast<T*>(obj);
         vec.push_back(tObj);
      }
   }
   else if ( state == PSS_UPDATE )
   {
      unsigned int count = vec.size();
      for (unsigned int i = 0; i < count; ++i)
      {
         Serializable* obj = dynamic_cast<Serializable*>(vec[i]);
         *this << &obj;
         vec[i] = dynamic_cast< T* >( obj );
      }
   }
}
///////////////////////////////////////////////////////////////////////////////

#endif // _SERIALIZER_H

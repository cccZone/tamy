#ifndef _SERIALIZER_H
#error "This file can only be included from Serializer.h"
#else

/// @file   core\ExternalTypesSerialization.h
/// @brief  serialization methods for the external data types

#include <vector>
#include "core\Serializer.h"
#include "core\SerializerImpl.h"


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
         T data;
         *this << data;
         vec.push_back( data );
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

template< typename T >
void DependenciesMapper::saveDependency( typename std::vector< T* >& depsMap, T* ptr )
{
   unsigned int count = depsMap.size();
   for ( unsigned int idx = 0; idx < count; ++idx )
   {
      if ( depsMap[idx] == ptr )
      {
         // yes - we have it mapped
         return;
      }
   }

   depsMap.push_back( ptr );
   ptr->save( *this );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _SERIALIZER_H

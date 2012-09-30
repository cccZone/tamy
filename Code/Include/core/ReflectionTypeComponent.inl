#ifndef _REFLECTION_TYPE_COMPONENT
#error "This file can only be included from ReflectionTypeComponent.h"
#else

#include "core/StringUtils.h"
#include "core/ReflectionProperty.h"


///////////////////////////////////////////////////////////////////////////////

uint ReflectionTypeComponent::generateId( const std::string& memberName )
{
   return StringUtils::calculateHash( memberName );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ReflectionTypeComponent::savePtr( const T* dataPtr, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const
{
   bool isPtrNull = (dataPtr == NULL);
   stream << isPtrNull;

   if ( dataPtr != NULL )
   {
      uint dependencyIdx = dependenciesMapper.findDependency( reinterpret_cast< const ReflectionObject* >( dataPtr ) );
      stream << dependencyIdx;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
T* ReflectionTypeComponent::loadPtr( InStream& stream ) const
{
   bool isPtrNull;
   stream >> isPtrNull;
   if ( isPtrNull )
   {
      return NULL;
   }
   else
   {
      uint dependencyIdx;
      stream >> dependencyIdx;

      // the dependency will be mapped later on
      return reinterpret_cast< T* >( dependencyIdx );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
TMemberField< T >::TMemberField( const std::string& memberName, int offset ) 
   : ReflectionTypeComponent( memberName )
   , m_dataOffset( offset )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< T >::save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const
{
   const char* memberPtr = (const char*)object + m_dataOffset;
   const T* dataPtr = reinterpret_cast< const T* >( memberPtr );
   stream << *dataPtr;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< T >::load( void* object, InStream& stream ) const 
{
   T tmpData;
   stream >> tmpData;
   if ( object )
   {
      char* memberPtr = (char*)object + m_dataOffset;
      T* dataPtr = reinterpret_cast< T* >( memberPtr );
      *dataPtr = tmpData;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< T >::mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const
{
   // there are no dependencies among simple types, just pointers
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< T >::restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const
{
   // there are no dependencies among simple types, just pointers
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
ReflectionProperty* TMemberField< T >::instantiateProperty( void* object ) const
{
   char* memberPtr = (char*)object + m_dataOffset;
   T* dataPtr = reinterpret_cast< T* >( memberPtr );

   return new TReflectionProperty< T >( (ReflectionObject*)object, dataPtr );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
TMemberField< T* >::TMemberField( const std::string& memberName, int offset ) 
   : ReflectionTypeComponent( memberName )
   , m_dataOffset( offset )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< T* >::save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const
{
   const char* memberPtr = (const char*)object + m_dataOffset;
   T* const * dataPtr = reinterpret_cast< T* const * >( memberPtr );
   savePtr( *dataPtr, dependenciesMapper, stream );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< T* >::load( void* object, InStream& stream ) const 
{
   T* restoredPtr = loadPtr< T >( stream );

   if ( object )
   {
      char* memberPtr = (char*)object + m_dataOffset;
      T** dataPtr = reinterpret_cast< T** >( memberPtr );
      *dataPtr = restoredPtr;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< T* >::mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const
{
   const char* memberPtr = (const char*)object + m_dataOffset;
   T* const * dataPtr = reinterpret_cast< T* const * >( memberPtr );

   // add the stored object as a dependency
   const T* storedObject = static_cast< const T* >( *dataPtr ) ;
   dependenciesCollector.addDependency( storedObject );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< T* >::restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const
{
   char* memberPtr = (char*)object + m_dataOffset;
   T** dataPtr = reinterpret_cast< T** >( memberPtr );

   // the pointer contains a dependency index - now we just have to find a corresponding object
   bool wasRestored = false;
   T* restoredObject = reinterpret_cast< T* >( dependenciesLinker.findDependency( (uint)(*dataPtr), wasRestored ) );
   *dataPtr = restoredObject;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
ReflectionProperty* TMemberField< T* >::instantiateProperty( void* object ) const
{
   char* memberPtr = (char*)object + m_dataOffset;
   T** dataPtr = reinterpret_cast< T** >( memberPtr );

   return new TReflectionProperty< T* >( (ReflectionObject*)object, dataPtr );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
TMemberField< TRefPtr< T > >::TMemberField( const std::string& memberName, int offset ) 
   : ReflectionTypeComponent( memberName )
   , m_dataOffset( offset )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< TRefPtr< T > >::save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const
{
   const char* memberPtr = (const char*)object + m_dataOffset;
   TRefPtr< T > const * dataPtr = reinterpret_cast< TRefPtr< T > const * >( memberPtr );
   savePtr( dataPtr->get(), dependenciesMapper, stream );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< TRefPtr< T > >::load( void* object, InStream& stream ) const 
{
   T* restoredPtr = loadPtr< T >( stream );

   if ( object )
   {
      char* memberPtr = (char*)object + m_dataOffset;
      TRefPtr< T >* dataPtr = reinterpret_cast< TRefPtr< T >* >( memberPtr );
      dataPtr->setDuringSerialization( restoredPtr );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< TRefPtr< T > >::mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const
{
   const char* memberPtr = (const char*)object + m_dataOffset;
   TRefPtr< T > const * dataPtr = reinterpret_cast< TRefPtr< T > const * >( memberPtr );

   // add the stored object as a dependency
   const T* storedObject = static_cast< const T* >( dataPtr->get() ) ;
   dependenciesCollector.addDependency( storedObject );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< TRefPtr< T > >::restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const
{
   char* memberPtr = (char*)object + m_dataOffset;
   TRefPtr< T >* dataPtr = reinterpret_cast< TRefPtr< T >* >( memberPtr );

   // the pointer contains a dependency index - now we just have to find a corresponding object
   bool wasRestored = false;
   T* restoredObject = reinterpret_cast< T* >( dependenciesLinker.findDependency( (uint)( dataPtr->get() ), wasRestored ) );
   if ( wasRestored )
   {
      restoredObject->addReference();
   }
   dataPtr->setDuringSerialization( restoredObject );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
ReflectionProperty* TMemberField< TRefPtr< T > >::instantiateProperty( void* object ) const
{
   char* memberPtr = (char*)object + m_dataOffset;
   T** dataPtr = reinterpret_cast< T** >( memberPtr );

   return new TReflectionProperty< T* >( (ReflectionObject*)object, dataPtr );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
TMemberField< std::vector< T > >::TMemberField( const std::string& memberName, int offset ) 
   : ReflectionTypeComponent( memberName )
   , m_dataOffset( offset )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< std::vector< T > >::save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const
{
   const char* memberPtr = (const char*)object + m_dataOffset;
   const std::vector< T >* dataPtr = reinterpret_cast< const std::vector< T >* >( memberPtr );

   // serialize the number of entries
   uint count = dataPtr->size();
   stream << count;

   // serialize the entires
   for ( uint i = 0; i < count; ++i )
   {
      stream << (*dataPtr)[i];
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< std::vector< T > >::load( void* object, InStream& stream ) const 
{
   char* memberPtr = (char*)object + m_dataOffset;
   std::vector< T >* dataPtr = reinterpret_cast< std::vector< T >* >( memberPtr );

   // deserialize the number of entries
   uint count;
   stream >> count;

   // make place in the array
   if ( object )
   {
      dataPtr->resize( count );
   }

   // deserialize the entries
   for ( uint i = 0; i < count; ++i )
   {
      stream >> (*dataPtr)[i];
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< std::vector< T > >::mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const
{
   // there are no dependencies among simple types, just pointers
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< std::vector< T > >::restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const
{
   // there are no dependencies among simple types, just pointers
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
ReflectionProperty* TMemberField< std::vector< T > >::instantiateProperty( void* object ) const
{
   char* memberPtr = (char*)object + m_dataOffset;
   std::vector< T >* dataPtr = reinterpret_cast< std::vector< T >* >( memberPtr );

   return new TReflectionProperty< std::vector< T > >( (ReflectionObject*)object, dataPtr );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
TMemberField< std::vector< T* > >::TMemberField( const std::string& memberName, int offset ) 
   : ReflectionTypeComponent( memberName )
   , m_dataOffset( offset )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< std::vector< T* > >::save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const
{
   const char* memberPtr = (const char*)object + m_dataOffset;
   const std::vector< T* >* dataPtr = reinterpret_cast< const std::vector< T* >* >( memberPtr );

   // serialize the number of entries
   uint count = dataPtr->size();
   stream << count;

   // serialize the entires
   for ( uint i = 0; i < count; ++i )
   {
      // serialize the pointer
      savePtr( (*dataPtr)[i], dependenciesMapper, stream );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< std::vector< T* > >::load( void* object, InStream& stream ) const 
{
   char* memberPtr = (char*)object + m_dataOffset;
   std::vector< T* >* dataPtr = reinterpret_cast< std::vector< T* >* >( memberPtr );

   // deserialize the number of entries
   uint count;
   stream >> count;

   // make place in the array
   if ( object )
   {
      dataPtr->resize( count, NULL );
   }

   // deserialize the entries
   for ( uint i = 0; i < count; ++i )
   {
      // deserialize the pointer
      T* restoredPtr = loadPtr< T >( stream );
      if ( object )
      {
         (*dataPtr)[i] = restoredPtr;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< std::vector< T* > >::mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const
{
   const char* memberPtr = (const char*)object + m_dataOffset;
   const std::vector< T* >* dataPtr = reinterpret_cast< const std::vector< T* >* >( memberPtr );

   uint count = dataPtr->size();
   for ( uint i = 0; i < count; ++i )
   {
      // add the stored object as a dependency
      const T* storedObject = reinterpret_cast< const T* >( (*dataPtr)[i] ) ;
      dependenciesCollector.addDependency( storedObject );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< std::vector< T* > >::restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const
{
   char* memberPtr = (char*)object + m_dataOffset;
   std::vector< T* >* dataPtr = reinterpret_cast< std::vector< T* >* >( memberPtr );

   uint count = dataPtr->size();
   for ( uint i = 0; i < count; ++i )
   {
      // the pointers contain indices to the actual dependencies - now we just have to find corresponding objects
      bool wasRestored = false;
      T* restoredObject = reinterpret_cast< T* >( dependenciesLinker.findDependency( (uint)(*dataPtr)[i], wasRestored ) );
      (*dataPtr)[i] = restoredObject;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
ReflectionProperty* TMemberField< std::vector< T* > >::instantiateProperty( void* object ) const
{
   char* memberPtr = (char*)object + m_dataOffset;
   std::vector< T* >* dataPtr = reinterpret_cast< std::vector< T* >* >( memberPtr );

   return new TReflectionProperty< std::vector< T* > >( (ReflectionObject*)object, dataPtr );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
TMemberField< std::vector< TRefPtr< T > > >::TMemberField( const std::string& memberName, int offset ) 
   : ReflectionTypeComponent( memberName )
   , m_dataOffset( offset )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< std::vector< TRefPtr< T > > >::save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const
{
   const char* memberPtr = (const char*)object + m_dataOffset;
   const std::vector< TRefPtr< T > >* dataPtr = reinterpret_cast< const std::vector< TRefPtr< T > >* >( memberPtr );

   // serialize the number of entries
   uint count = dataPtr->size();
   stream << count;

   // serialize the entires
   for ( uint i = 0; i < count; ++i )
   {
      // serialize the pointer
      savePtr( (*dataPtr)[i].get(), dependenciesMapper, stream );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< std::vector< TRefPtr< T > > >::load( void* object, InStream& stream ) const 
{
   char* memberPtr = (char*)object + m_dataOffset;
   std::vector< TRefPtr< T > >* dataPtr = reinterpret_cast< std::vector< TRefPtr< T > >* >( memberPtr );

   // deserialize the number of entries
   uint count;
   stream >> count;

   // make place in the array
   if ( object )
   {
      dataPtr->resize( count, NULL );
   }

   // deserialize the entries
   for ( uint i = 0; i < count; ++i )
   {
      // deserialize the pointer
      T* restoredPtr = loadPtr< T >( stream );
      if ( object )
      {
         (*dataPtr)[i].setDuringSerialization( restoredPtr );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< std::vector< TRefPtr< T > > >::mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const
{
   const char* memberPtr = (const char*)object + m_dataOffset;
   const std::vector< TRefPtr< T > >* dataPtr = reinterpret_cast< const std::vector< TRefPtr< T > >* >( memberPtr );

   uint count = dataPtr->size();
   for ( uint i = 0; i < count; ++i )
   {
      // add the stored object as a dependency
      const T* storedObject = reinterpret_cast< const T* >( (*dataPtr)[i].get() ) ;
      dependenciesCollector.addDependency( storedObject );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TMemberField< std::vector< TRefPtr< T > > >::restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const
{
   char* memberPtr = (char*)object + m_dataOffset;
   std::vector< TRefPtr< T > >* dataPtr = reinterpret_cast< std::vector< TRefPtr< T > >* >( memberPtr );

   uint count = dataPtr->size();
   for ( uint i = 0; i < count; ++i )
   {
      // the pointers contain indices to the actual dependencies - now we just have to find corresponding objects
      bool wasRestored = false;
      T* restoredObject = reinterpret_cast< T* >( dependenciesLinker.findDependency( (uint)(*dataPtr)[i].get(), wasRestored ) );
      if ( wasRestored )
      {
         restoredObject->addReference();
      }
      (*dataPtr)[i].setDuringSerialization( restoredObject );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
ReflectionProperty* TMemberField< std::vector< TRefPtr< T > > >::instantiateProperty( void* object ) const
{
   char* memberPtr = (char*)object + m_dataOffset;
   std::vector< TRefPtr< T > >* dataPtr = reinterpret_cast< std::vector< TRefPtr< T > >* >( memberPtr );

   return new TReflectionProperty< std::vector< TRefPtr< T > > >( (ReflectionObject*)object, dataPtr );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_TYPE_COMPONENT

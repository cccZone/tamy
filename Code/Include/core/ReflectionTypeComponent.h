/// @file   core/ReflectionTypeComponent.h
/// @brief  a class member description
#ifndef _REFLECTION_TYPE_COMPONENT
#define _REFLECTION_TYPE_COMPONENT

#include "core\MemoryRouter.h"
#include "core\types.h"
#include "core\RefPtr.h"
#include "core\Array.h"
#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class ReflectionDependencyMapperCallback;
class ReflectionDependencyLinkerCallback;
class ReflectionProperty;
class OutStream;
class InStream;

///////////////////////////////////////////////////////////////////////////////

/**
 * A class member description.
 */
class ReflectionTypeComponent
{
   DECLARE_ALLOCATOR( ReflectionTypeComponent, AM_DEFAULT );

public:
   std::string                m_memberName;
   uint                       m_id;

   bool                       m_isEditable;
   std::string                m_label;          // name of the field displayed in an external editor
   bool                       m_canBeSaved;

public:
   virtual ~ReflectionTypeComponent() {}

   /**
    * Mark the field as editable or read-only. Even if its read only, you'll
    * be able to change its value, but when the field is editable, it means
    * that when you attach a properties view, it will be allowed to create
    * a custom external editor for the field.
    *
    * @param label
    */
   inline ReflectionTypeComponent* setEditable( const char* label ) { m_isEditable = true; m_label = label; return this; }

   /**
    * Marks the field as non-savable - it's contents can be viewed, modified at runtime, but they
    * won't be serialized.
    */
   inline ReflectionTypeComponent* setRuntimeOnly() { m_canBeSaved = false; return this; }

   /**
    * Generates an id the member with the specified name will be identified by.
    *
    * @param   memberName
    * @return  unique member id
    */
   static inline uint generateId( const std::string& memberName );

   /**
      * Serializes the data the member stores.
      *
      * @param object                  pointer to the host object
      * @param dependenciesMapper      dependencies ( embedded objects ) mapper
      * @param stream                  output data stream to which the object should be serialized
      */
   virtual void save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const = 0;

   /**
    * Deserializes the the member's data.
    *
    * @param object                    pointer to the host object
    * @param stream                    input data stream that contains the serialized object
    */
   virtual void load( void* object, InStream& stream ) const = 0;

   /**
    * Maps the dependencies on the objects the specified object references.
    *
    * @param object                    mapped object
    * @param dependenciesMapper        dependencies (embedded objects) collector
    */
   virtual void mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const = 0;

   /**
    * Restores the dependencies on the objects the specified object references.
    *
    * @param object                    restored object
    * @param dependenciesLinker        dependencies (embedded objects) linker
    */
   virtual void restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const = 0;

   /**
    * Creates a property for this type member of the specified object.
    *
    * @param object
    * @param outProperties
    *
    * @return reference to the created property
    */
   ReflectionProperty* createProperty( void* object ) const;


protected:
   /**
    * Constructor.
    *
    * @param memberName
    */
   ReflectionTypeComponent( const std::string& memberName );

   /**
    * A helper method for serializing a pointer.
    *
    * @param dataPtr                   pointer to serialize
    * @param dependenciesMapper        dependencies ( embedded objects ) mapper
    * @param stream                    stream to serialize the pointer in
    */
   template< typename T >
   void savePtr( const T* dataPtr, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const;

   /**
    * A helper method for deserializing a pointer.
    *
    * @param dataPtr                   pointer to deserialize
    * @param stream                    stream to deserialize the pointer from
    */
   template< typename T >
   T* loadPtr( InStream& stream ) const;

   /**
    * Instantiates a property for this type member of the specified object.
    *
    * @param object
    * @param outProperties
    *
    * @return reference to the created property
    */
   virtual ReflectionProperty* instantiateProperty( void* object ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type-specific class member description for serializing POD types.
 */
template< typename T >
class TMemberField : public ReflectionTypeComponent
{
   DECLARE_ALLOCATOR( TMemberField, AM_DEFAULT );

private:
   int               m_dataOffset;

public:
   TMemberField( const std::string& memberName, int offset );

   // -------------------------------------------------------------------------
   // ReflectionTypeComponent implementation
   // -------------------------------------------------------------------------
   void save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const;
   void load( void* object, InStream& stream ) const;
   void mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const;
   void restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const;
   ReflectionProperty* instantiateProperty( void* object ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type-specific class member description for serializing pointer types.
 */
template< typename T >
class TMemberField< T* > : public ReflectionTypeComponent
{
   DECLARE_ALLOCATOR( TMemberField, AM_DEFAULT );

private:
   int               m_dataOffset;

public:
   TMemberField( const std::string& memberName, int offset );

   // -------------------------------------------------------------------------
   // ReflectionTypeComponent implementation
   // -------------------------------------------------------------------------
   void save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const;
   void load( void* object, InStream& stream ) const;
   void mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const;
   void restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const;
   ReflectionProperty* instantiateProperty( void* object ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type-specific class member description for serializing reference counted pointers.
 */
template< typename T >
class TMemberField< TRefPtr< T > > : public ReflectionTypeComponent
{
   DECLARE_ALLOCATOR( TMemberField, AM_DEFAULT );

private:
   int               m_dataOffset;

public:
   TMemberField( const std::string& memberName, int offset );

   // -------------------------------------------------------------------------
   // ReflectionTypeComponent implementation
   // -------------------------------------------------------------------------
   void save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const;
   void load( void* object, InStream& stream ) const;
   void mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const;
   void restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const;
   ReflectionProperty* instantiateProperty( void* object ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type-specific class member description for serializing arrays of POD types.
 */
template< typename T >
class TMemberField< std::vector< T > > : public ReflectionTypeComponent
{
   DECLARE_ALLOCATOR( TMemberField, AM_DEFAULT );

private:
   int               m_dataOffset;

public:
   TMemberField( const std::string& memberName, int offset );

   // -------------------------------------------------------------------------
   // ReflectionTypeComponent implementation
   // -------------------------------------------------------------------------
   void save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const;
   void load( void* object, InStream& stream ) const;
   void mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const;
   void restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const;
   ReflectionProperty* instantiateProperty( void* object ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type-specific class member description for serializing arrays of pointers.
 */
template< typename T >
class TMemberField< std::vector< T* > > : public ReflectionTypeComponent
{
   DECLARE_ALLOCATOR( TMemberField, AM_DEFAULT );

private:
   int               m_dataOffset;

public:
   TMemberField( const std::string& memberName, int offset );

   // -------------------------------------------------------------------------
   // ReflectionTypeComponent implementation
   // -------------------------------------------------------------------------
   void save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const;
   void load( void* object, InStream& stream ) const;
   void mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const;
   void restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const;
   ReflectionProperty* instantiateProperty( void* object ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type-specific class member description for serializing arrays of reference counted pointers.
 */
template< typename T >
class TMemberField< std::vector< TRefPtr< T > > > : public ReflectionTypeComponent
{
   DECLARE_ALLOCATOR( TMemberField, AM_DEFAULT );

private:
   int               m_dataOffset;

public:
   TMemberField( const std::string& memberName, int offset );

   // -------------------------------------------------------------------------
   // ReflectionTypeComponent implementation
   // -------------------------------------------------------------------------
   void save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const;
   void load( void* object, InStream& stream ) const;
   void mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const;
   void restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const;
   ReflectionProperty* instantiateProperty( void* object ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type-specific class member description for serializing arrays of POD types.
 */
template< typename T >
class TMemberField< Array< T > > : public ReflectionTypeComponent
{
   DECLARE_ALLOCATOR( TMemberField, AM_DEFAULT );

private:
   int               m_dataOffset;

public:
   TMemberField( const std::string& memberName, int offset );

   // -------------------------------------------------------------------------
   // ReflectionTypeComponent implementation
   // -------------------------------------------------------------------------
   void save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const;
   void load( void* object, InStream& stream ) const;
   void mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const;
   void restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const;
   ReflectionProperty* instantiateProperty( void* object ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type-specific class member description for serializing arrays of pointers.
 */
template< typename T >
class TMemberField< Array< T* > > : public ReflectionTypeComponent
{
   DECLARE_ALLOCATOR( TMemberField, AM_DEFAULT );

private:
   int               m_dataOffset;

public:
   TMemberField( const std::string& memberName, int offset );

   // -------------------------------------------------------------------------
   // ReflectionTypeComponent implementation
   // -------------------------------------------------------------------------
   void save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const;
   void load( void* object, InStream& stream ) const;
   void mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const;
   void restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const;
   ReflectionProperty* instantiateProperty( void* object ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type-specific class member description for serializing arrays of reference counted pointers.
 */
template< typename T >
class TMemberField< Array< TRefPtr< T > > > : public ReflectionTypeComponent
{
   DECLARE_ALLOCATOR( TMemberField, AM_DEFAULT );

private:
   int               m_dataOffset;

public:
   TMemberField( const std::string& memberName, int offset );

   // -------------------------------------------------------------------------
   // ReflectionTypeComponent implementation
   // -------------------------------------------------------------------------
   void save( const void* object, const ReflectionDependencyMapperCallback& dependenciesMapper, OutStream& stream ) const;
   void load( void* object, InStream& stream ) const;
   void mapDependencies( const void* object, ReflectionDependencyMapperCallback& dependenciesCollector ) const;
   void restoreDependencies( void* object, const ReflectionDependencyLinkerCallback& dependenciesLinker ) const;
   ReflectionProperty* instantiateProperty( void* object ) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core/ReflectionTypeComponent.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_TYPE_COMPONENT

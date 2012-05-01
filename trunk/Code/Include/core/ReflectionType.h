/// @file   core/ReflectionType.h
/// @brief  reflection type definition
#ifndef _REFLECTION_TYPE_H
#define _REFLECTION_TYPE_H

#include <vector>
#include <string>
#include <map>
#include <list>
#include "core/types.h"


///////////////////////////////////////////////////////////////////////////////

class ReflectionSaver;
class ReflectionLoader;
class ReflectionTypeComponent;
class ReflectionProperties;
class OutStream;
class InStream;

///////////////////////////////////////////////////////////////////////////////

/**
 * Definition of a type that can be recognized at runtime and can serialize
 * objects defined using it.
 */
class ReflectionType
{
public:
   // type name and its id version
   std::string                                        m_name;
   unsigned int                                       m_id;

public:
   /**
    * Constructor.
    *
    * @param name             type name
    */
   ReflectionType( const std::string& name );
   virtual ~ReflectionType() {}

   /**
    * A utility method for generating a unique ID based on the specified type name.
    *
    * @param typeName
    */
   static inline uint generateId( const std::string& typeName );

   // ----------------------------------------------------------------------
   // Comparison
   // ----------------------------------------------------------------------
   
   /**
    * Checks if this type can be downcast onto the specified reference type.
    *
    * @param referenceType
    */
   virtual bool isA( const ReflectionType& referenceType ) const;

   /**
    * Checks if this type can exactly matches the specified reference type.
    *
    * @param referenceType
    */
   inline bool isExactlyA( const ReflectionType& referenceType ) const;
};

///////////////////////////////////////////////////////////////////////////////

struct SerializableTypeInstantiator
{
   virtual ~SerializableTypeInstantiator() {}

   virtual void* instantiate() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

template< typename T >
struct TSerializableTypeInstantiator : public SerializableTypeInstantiator
{
   void* instantiate() const;
};

/**
 * Definition of a type that can be recognized at runtime and can serialize
 * objects defined using it.
 */
class SerializableReflectionType : public ReflectionType
{
public:
   std::string                                        m_patchedName;
   unsigned int                                       m_patchedId;

private:
   SerializableTypeInstantiator*                      m_instantiator;
   std::vector< ReflectionTypeComponent* >            m_memberFields;

   typedef std::map< uint, uint >                     NamesMap;
   NamesMap                                           m_patchedMemberNames;

   std::vector< uint >                                m_baseTypesIds;

public:
   /**
    * Constructor.
    *
    * @param name             type name
    * @param patchedName      name of the type this type patches
    */
   SerializableReflectionType( const std::string& name, const std::string& patchedName = "" );
   ~SerializableReflectionType();

   // ----------------------------------------------------------------------
   // Instantiation
   // ----------------------------------------------------------------------

   /**
    * Sets up an instantiator of the represented type.
    */
   void setupInstantiator( SerializableTypeInstantiator* instantiator );

   /**
    * Creates an instance of this type.
    */
   template< typename T >
   T* instantiate() const;

   // ----------------------------------------------------------------------
   // Type definition
   // ----------------------------------------------------------------------

   /**
    * Defines a base type of this type ( in order to set up an inheritance hierarchy ).
    *
    * @param baseTypeName
    */
   void addBaseType( const std::string& baseTypeName );

   /**
    * Adds a new type member field.
    *
    * @param member     type member field description
    */
   void addMemberField( ReflectionTypeComponent* member );

   /**
    * Looks for a member with the specified member id.
    *
    * @param memberId
    * @return     pointer to the member definition, or NULL if such a member is not defined
    */
   ReflectionTypeComponent* findMemberField( uint memberId ) const;

   /**
    * Creates the properties for this type and the specified object.
    *
    * @param hostObj
    * @param outProperties
    */
   void collectProperties( void* hostObj, ReflectionProperties& outProperties ) const;

   /**
    * Tells if the type is abstract or not ( can it be instantiated or not ).
    */
   inline bool isAbstract() const;

   /**
    * Collects the parent types of this type.
    */
   void collectParents( std::vector< const SerializableReflectionType* >& outParentTypes ) const;

   /**
    * The opposite of the `collectParents` method - collects the types to which
    * this type is a parent.
    *
    * @param outDerivedTypes
    */
   void collectImplementations( std::vector< const SerializableReflectionType* >& outDerivedTypes ) const;

   // ----------------------------------------------------------------------
   // Patching support
   // ----------------------------------------------------------------------

   /**
    * Sets up a patching information for a member field.
    *
    * CATION: the member types MUST match, otherwise the patching won't work
    * 
    * @param oldMemberName    how the member used to be named
    * @param newMemberName    what's the new member name
    */
   void patchMemberField( const std::string& oldMemberName, const std::string& newMemberName );

   // ----------------------------------------------------------------------
   // Serialization support
   // ----------------------------------------------------------------------

   /**
    * Serializes an object representation that corresponds to this type definition.
    *
    * @param object              serialized object
    * @param dependenciesMapper  dependencies (embedded objects) mapper
    * @param stream              output data stream to which the object should be serialized
    */
   template< typename T >
   void save( const T& object, const ReflectionSaver& dependenciesMapper, OutStream& stream ) const;

   /**
    * Deserializes an object representation that corresponds to this type definition.
    *
    * @param stream           input data stream that contains the serialized object
    */
   template< typename T >
   static T* load( InStream& stream );

   /**
    * Maps the dependencies on the objects the specified object references.
    *
    * @param object              mapped object
    * @param dependenciesMapper  dependencies (embedded objects) collector
    */
   template< typename T >
   void mapDependencies( const T& object, ReflectionSaver& dependenciesCollector ) const;

   /**
    * Restores the dependencies on the objects the specified object references.
    *
    * @param object              restored object
    * @param dependenciesMapper  dependencies (embedded objects) mapper
    */
   template< typename T >
   void restoreDependencies( T& object, const ReflectionLoader& dependenciesMapper ) const;

   // ----------------------------------------------------------------------
   // ReflectionType implementation
   // ----------------------------------------------------------------------
   bool isA( const ReflectionType& referenceType ) const;

private:
   template< typename T >
   void saveMemberFields( const T& object, const ReflectionSaver& dependenciesMapper, OutStream& stream ) const;

   template< typename T >
   void loadMemberFields( T& object, InStream& stream ) const;

   void mapTypesHierarchy( std::list< const SerializableReflectionType* >& outReflectionTypesList ) const;

};

///////////////////////////////////////////////////////////////////////////////

/**
 * Returns an offset of the specified class member.
 */
#define MEMBER_OFFSET( OBJECT, MEMBER_NAME ) int( reinterpret_cast< long >( &(OBJECT)->MEMBER_NAME ) ) )

///////////////////////////////////////////////////////////////////////////////

#include "core/ReflectionType.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_TYPE_H

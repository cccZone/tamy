#include "core/ReflectionType.h"
#include "core/ReflectionTypeComponent.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

ReflectionType::ReflectionType( const std::string& name ) 
   : m_name( name )
{
   m_id = generateId( m_name );
}

///////////////////////////////////////////////////////////////////////////////

bool ReflectionType::isA( const ReflectionType& referenceType ) const
{
   return isExactlyA( referenceType );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SerializableReflectionType::SerializableReflectionType( const std::string& name, const std::string& patchedName ) 
   : ReflectionType( name )
   , m_patchedName( patchedName )
   , m_instantiator( NULL )
   , m_patchedId( -1 )
{
   if ( !m_patchedName.empty() )
   {
      m_patchedId = generateId( m_patchedName );
   }
}

///////////////////////////////////////////////////////////////////////////////

SerializableReflectionType::~SerializableReflectionType()
{
   delete m_instantiator; m_instantiator = NULL;

   unsigned int count = m_memberFields.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_memberFields[i];
   }
   m_memberFields.clear();
}

///////////////////////////////////////////////////////////////////////////////

void SerializableReflectionType::setupInstantiator( SerializableTypeInstantiator* instantiator )
{
   delete m_instantiator;
   m_instantiator = instantiator;
}

///////////////////////////////////////////////////////////////////////////////

bool SerializableReflectionType::isA( const ReflectionType& referenceType ) const
{
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();

   // explore the inheritance hierarchy of this type and verify
   // if any type in it matches the specified reference type
   std::list< const SerializableReflectionType* > bfs;
   bfs.push_back( this );
   while ( !bfs.empty() )
   {
      const SerializableReflectionType* currType = bfs.front();
      bfs.pop_front();

      if ( currType->m_id == referenceType.m_id )
      {
         return true;
      }

      // gather the base types
      uint childrenCount = currType->m_baseTypesIds.size();
      for ( uint i = 0; i < childrenCount; ++i )
      {
         const SerializableReflectionType* parentType = typesRegistry.findSerializable( currType->m_baseTypesIds[i] );
         if ( parentType )
         {
            bfs.push_back( parentType );
         }
      }
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

void  SerializableReflectionType::addBaseType( const std::string& baseTypeName )
{
   uint typeId = generateId( baseTypeName );

   // make sure the added id is unique
   uint count = m_baseTypesIds.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_baseTypesIds[i] == typeId )
      {
         ASSERT_MSG( m_baseTypesIds[i] != typeId, "Trying to redefine a base type" );
         return;
      }
   }

   m_baseTypesIds.push_back( typeId );
}

///////////////////////////////////////////////////////////////////////////////

void SerializableReflectionType::addMemberField( ReflectionTypeComponent* member )
{
   if ( member )
   {
      m_memberFields.push_back( member );
   }
}

///////////////////////////////////////////////////////////////////////////////

ReflectionTypeComponent* SerializableReflectionType::findMemberField( const std::string& memberName ) const
{
   uint memberId = ReflectionTypeComponent::generateId( memberName );
   ReflectionTypeComponent* foundComponent = findMemberField( memberId );
   return foundComponent;
}

///////////////////////////////////////////////////////////////////////////////

ReflectionTypeComponent* SerializableReflectionType::findMemberField( uint memberId ) const
{
   // check if this is not one of the patched members
   NamesMap::const_iterator it = m_patchedMemberNames.find( memberId );
   if ( it != m_patchedMemberNames.end() )
   {
      // it is - get the proper id
      memberId = it->second;
   }

   // find the member with a matching id
   uint count = m_memberFields.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_memberFields[i]->m_id == memberId )
      {
         // found it
         return m_memberFields[i];
      }
   }

   // a member with such id is not defined
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SerializableReflectionType::patchMemberField( const std::string& oldMemberName, const std::string& newMemberName )
{
   uint oldMemberId = ReflectionTypeComponent::generateId( oldMemberName );
   uint newMemberId = ReflectionTypeComponent::generateId( newMemberName );

   NamesMap::iterator it = m_patchedMemberNames.find( oldMemberId );
   if ( it != m_patchedMemberNames.end() )
   {
      ASSERT_MSG( false, "A member with this name already has a patching definition" );
   }
   else
   {
      m_patchedMemberNames.insert( std::make_pair( oldMemberId, newMemberId ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SerializableReflectionType::mapTypesHierarchy( std::list< const SerializableReflectionType* >& outReflectionTypesList ) const
{
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   std::vector< const SerializableReflectionType* > dfs;
   dfs.push_back( this );
   while( !dfs.empty() )
   {
      const SerializableReflectionType* nextType = dfs.back();
      dfs.pop_back();
      outReflectionTypesList.push_back( nextType );

      // go through the parent types
      uint parentTypesCount = nextType->m_baseTypesIds.size();
      for ( uint i = 0; i < parentTypesCount; ++i )
      {
         const SerializableReflectionType* parentType = typesRegistry.findSerializable( nextType->m_baseTypesIds[i] );
         if ( parentType )
         {
            dfs.push_back( parentType );
         }
         else
         {
            // we have a base type listed that's not registered - report that
            ASSERT_MSG( parentType != NULL, "Parent type that's not registered is listed" );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SerializableReflectionType::collectProperties( void* hostObj, std::vector< ReflectionProperty* >& outProperties ) const
{
   // get all types in the hierarchy
   std::list< const SerializableReflectionType* > reflectionTypesList;
   mapTypesHierarchy( reflectionTypesList );

   while( !reflectionTypesList.empty() )
   {
      // go through each one and query their members for properties
      const SerializableReflectionType* nextType = reflectionTypesList.front();
      reflectionTypesList.pop_front();

      int membersCount = nextType->m_memberFields.size();
      for ( int i = 0; i < membersCount; ++i )
      {
         const ReflectionTypeComponent& field = *( nextType->m_memberFields[i] );

         ReflectionProperty* property = field.createProperty( hostObj );

         // copy the field flags
         if ( property != NULL )
         {
            outProperties.push_back( property );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SerializableReflectionType::collectParents( std::vector< const SerializableReflectionType* >& outParentTypes ) const
{
   const ReflectionTypesRegistry& registry = ReflectionTypesRegistry::getInstance();
   
   uint count = m_baseTypesIds.size();
   for ( uint i = 0; i < count; ++i )
   {
      const SerializableReflectionType* parent = registry.findSerializable( m_baseTypesIds[i] );
      ASSERT_MSG( parent != NULL, "Parent of the type is not registered" );
      if ( parent != NULL )
      {
         outParentTypes.push_back( parent );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SerializableReflectionType::collectImplementations( std::vector< const SerializableReflectionType* >& outDerivedTypes, bool includeAbstractTypes ) const
{
   const ReflectionTypesRegistry& registry = ReflectionTypesRegistry::getInstance();

   std::vector< const SerializableReflectionType* > types;
   registry.getMatchingSerializableTypes( m_id, types, includeAbstractTypes );

   // remove self from the list
   uint count = types.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( types[i]->m_id == m_id )
      {
         types.erase( types.begin() + i );
         break;
      }
   }

   outDerivedTypes.insert( outDerivedTypes.end(), types.begin(), types.end() );
}

///////////////////////////////////////////////////////////////////////////////

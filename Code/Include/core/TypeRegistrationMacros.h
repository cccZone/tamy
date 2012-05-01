/// @file   core/TypeRegistrationMacros.h
/// @brief  macros you should use to register the reflection types
#pragma once

#include "core/ReflectionObject.h"
#include "core/ResourceHandle.h"



///////////////////////////////////////////////////////////////////////////////

/**
 * Reflection type registration macros.
 */
#ifndef IMPORT_RTTI_REGISTRATIONS
   #define REGISTER_TYPE( ClassType )
   #define REGISTER_ABSTRACT_TYPE( ClassType )
   #define PATCH_TYPE( PatchedType, NewType )
   #define PATCH_ABSTRACT_TYPE( PatchedType, NewType )
   #define REGISTER_EXTERNAL_TYPE( ClassType )
   #define REGISTER_ENUM_TYPE( ClassType )
   #define REGISTER_CORE_TYPES()
#else
   #define REGISTER_TYPE( ClassType )                                         \
   struct ClassType##RTTIImporter                                             \
   {                                                                          \
      ClassType##RTTIImporter()                                               \
      {                                                                       \
         ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance(); \
         typesRegistry.addSerializableType< ClassType >( #ClassType, new TSerializableTypeInstantiator< ClassType >() );  \
      }                                                                       \
   } registerClass##ClassType;                                                \
   DEFINE_TYPE_ID( ClassType );                                               \
   DEFINE_TYPE_ID( TResourceHandle< ClassType > );

   #define REGISTER_ABSTRACT_TYPE( ClassType )                                \
   struct ClassType##RTTIImporter                                             \
   {                                                                          \
      ClassType##RTTIImporter()                                               \
      {                                                                       \
         ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance(); \
         typesRegistry.addSerializableType< ClassType >( #ClassType, NULL ); \
      }                                                                       \
   } registerClass##ClassType;                                                \
   DEFINE_TYPE_ID( ClassType );                                               \
   DEFINE_TYPE_ID( TResourceHandle< ClassType > );

   #define PATCH_TYPE( PatchedType, NewType )                                 \
   struct PatchedType##NewType##RTTIImporter                                  \
   {                                                                          \
      PatchedType##NewType##RTTIImporter()                                    \
      {                                                                       \
         ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance(); \
         typesRegistry.addSerializableType< NewType >( #NewType, new TSerializableTypeInstantiator< ClassType >(), #PatchedType ); \
      }                                                                       \
   } registerClass##PatchedType##NewType;                                     \
   DEFINE_TYPE_ID( NewType );                                                 \
   DEFINE_TYPE_ID( TResourceHandle< NewType > );

   #define PATCH_ABSTRACT_TYPE( PatchedType, NewType )                        \
   struct PatchedType##NewType##RTTIImporter                                  \
   {                                                                          \
      PatchedType##NewType##RTTIImporter()                                    \
      {                                                                       \
         ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance(); \
         typesRegistry.addSerializableType< NewType >( #NewType, NULL, #PatchedType ); \
      }                                                                       \
   } registerClass##PatchedType##NewType;                                     \
   DEFINE_TYPE_ID( NewType );                                                 \
   DEFINE_TYPE_ID( TResourceHandle< NewType > );

   #define REGISTER_EXTERNAL_TYPE( ClassType )                                \
   struct ClassType##RTTIImporter                                             \
   {                                                                          \
      ClassType##RTTIImporter()                                               \
      {                                                                       \
         ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance(); \
         typesRegistry.addExternalType< ClassType >( #ClassType );            \
      }                                                                       \
   } registerClass##ClassType;                                                \
   DEFINE_TYPE_ID( ClassType );                                               \
   DEFINE_TYPE_ID( TResourceHandle< ClassType > );

   #define REGISTER_ENUM_TYPE( EnumType )                                     \
   struct EnumType##RTTIImporter                                              \
   {                                                                          \
      EnumType##RTTIImporter()                                                \
      {                                                                       \
         ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance(); \
         typesRegistry.addEnumType< EnumType >( #EnumType );                  \
      }                                                                       \
   } registerClass##EnumType;                                                 \
   DEFINE_TYPE_ID( EnumType );                                                \
   DEFINE_TYPE_ID( TResourceHandle< EnumType > );

   #define REGISTER_CORE_TYPES()                                              \
   struct __CoreTypesRTTIImporter                                             \
   {                                                                          \
      __CoreTypesRTTIImporter()                                               \
      {                                                                       \
         ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance(); \
         typesRegistry.addExternalType< int >( "int" );                       \
         typesRegistry.addExternalType< unsigned int >( "unsigned int" );     \
         typesRegistry.addExternalType< char >( "char" );                     \
         typesRegistry.addExternalType< unsigned char >( "unsigned char" );   \
         typesRegistry.addExternalType< long >( "long" );                     \
         typesRegistry.addExternalType< unsigned long >( "unsigned long" );   \
         typesRegistry.addExternalType< short >( "short" );                   \
         typesRegistry.addExternalType< unsigned short >( "unsigned short" ); \
         typesRegistry.addExternalType< bool >( "bool" );                     \
         typesRegistry.addExternalType< float >( "float" );                   \
         typesRegistry.addExternalType< double >( "double" );                 \
         typesRegistry.addExternalType< std::string >( "std::string" );       \
         typesRegistry.addExternalType< D3DXMATRIX >( "D3DXMATRIX" );         \
         typesRegistry.addExternalType< D3DXQUATERNION >( "D3DXQUATERNION" ); \
         typesRegistry.addExternalType< D3DXVECTOR2 >( "D3DXVECTOR2" );       \
         typesRegistry.addExternalType< D3DXVECTOR3 >( "D3DXVECTOR3" );       \
         typesRegistry.addExternalType< D3DXVECTOR4 >( "D3DXVECTOR4" );       \
      }                                                                       \
   } registerClass##EnumType;                                                 \
   DEFINE_TYPE_ID( int )                                                      \
   DEFINE_TYPE_ID( unsigned int )                                             \
   DEFINE_TYPE_ID( char )                                                     \
   DEFINE_TYPE_ID( unsigned char )                                            \
   DEFINE_TYPE_ID( long )                                                     \
   DEFINE_TYPE_ID( unsigned long )                                            \
   DEFINE_TYPE_ID( short )                                                    \
   DEFINE_TYPE_ID( unsigned short )                                           \
   DEFINE_TYPE_ID( bool )                                                     \
   DEFINE_TYPE_ID( float )                                                    \
   DEFINE_TYPE_ID( double )                                                   \
   DEFINE_TYPE_ID( std::string )                                              \
   DEFINE_TYPE_ID( D3DXMATRIX )                                               \
   DEFINE_TYPE_ID( D3DXQUATERNION )                                           \
   DEFINE_TYPE_ID( D3DXVECTOR2 )                                              \
   DEFINE_TYPE_ID( D3DXVECTOR3 )                                              \
   DEFINE_TYPE_ID( D3DXVECTOR4 )                                              \
   DEFINE_TYPE_ID( ReflectionEnum )                                           \
   DEFINE_TYPE_ID( ReflectionObject )

#endif

///////////////////////////////////////////////////////////////////////////////

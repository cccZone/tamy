/// @file   core\RefelctionMacros.h
/// @brief  macros used to define reflected types
#pragma once



///////////////////////////////////////////////////////////////////////////////
// MACROS
///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro inside a class's declaration body to indicate that 
 * we want it to be a part of the RTTI system.
 */
#define DECLARE_CLASS()                                                       \
   private:                                                                   \
      static SerializableReflectionType*       s_type;                        \
   public:                                                                    \
      virtual const SerializableReflectionType& getVirtualRTTI() const { return *s_type; } \
      static  const SerializableReflectionType& getStaticRTTI() { return *s_type; } \
      static void setupReflectionType( SerializableReflectionType& type );    \
   private:

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro inside a class's declaration body to indicate that 
 * we want it to be a part of the RTTI system.
 */
#define DECLARE_STRUCT()                                                      \
   private:                                                                   \
      static SerializableReflectionType*       s_type;                        \
   public:                                                                    \
      virtual const SerializableReflectionType& getVirtualRTTI() const { return *s_type; } \
      static  const SerializableReflectionType& getStaticRTTI() { return *s_type; } \
      static void setupReflectionType( SerializableReflectionType& type );    \
   public:

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro to register a class with the reflection system.
 */
#define BEGIN_OBJECT( ClassType )                                             \
   SerializableReflectionType* ClassType::s_type;                             \
   void ClassType::setupReflectionType( SerializableReflectionType& type )    \
   {                                                                          \
      s_type = &type;                                                         \
      ClassType* objPtr = reinterpret_cast< ClassType* >( NULL );

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro to register an abstract class with the reflection system.
 */
#define BEGIN_ABSTRACT_OBJECT( ClassType )                                    \
   SerializableReflectionType* ClassType::s_type;                             \
   void ClassType::setupReflectionType( SerializableReflectionType& type )    \
   {                                                                          \
      s_type = &type;                                                         \
      ClassType* objPtr = reinterpret_cast< ClassType* >( NULL );

///////////////////////////////////////////////////////////////////////////////


/**
 * This macro ends the RTTI type implementation
 */
#define END_OBJECT() }

///////////////////////////////////////////////////////////////////////////////

/**
 * Declares the parent type this type derives from
 */
#define PARENT( ParentClassType )                                             \
   s_type->addBaseType( #ParentClassType );


///////////////////////////////////////////////////////////////////////////////

/**
 * Adds a patching information about one of the type's member fields.
 *
 * @param oldMember     member that used to be there, but was removed
 * @param newMember     member that was put in instead of the 'oldMember'
 */
#define PATCH_MEMBER( oldMember, newMember )                                  \
   s_type->patchMemberField( #oldMember, #newMember );

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro defines a non-editable member field.
 */
#define PROPERTY( type, variable )                                            \
   s_type->addMemberField( new TMemberField< type >( #variable, MEMBER_OFFSET( objPtr, variable ) );

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro defines an editable member field.
 */
#define PROPERTY_EDIT( label, type, variable )                                \
   s_type->addMemberField( ( new TMemberField< type >( #variable, MEMBER_OFFSET( objPtr, variable ) )->setEditable( #label ) );

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro defines an editable member field that will not be saved ( is valid
 * only for one runtime ).
 */
#define PROPERTY_EDIT_RUNTIME( label, type, variable )                        \
   s_type->addMemberField( ( new TMemberField< type >( #variable, MEMBER_OFFSET( objPtr, variable ) )->setEditable( #label )->setRuntimeOnly() );

///////////////////////////////////////////////////////////////////////////////

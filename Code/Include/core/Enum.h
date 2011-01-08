/// @file   core/Enum.h
/// @brief  serializable enums convertible to strings.
//          NOTE: The credit for this code goes to Francis Xavier Joseph Pulikotil, based on
//          whose code this implementation was derived
#ifndef _STRING_ENUM_H
#define _STRING_ENUM_H

#include <string>
#include <map>
#include "core/Class.h"
#include "core/TypeID.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * An abstract enum RTTI type.
 */
class Enum
{
   DECLARE_RTTI_CLASS 

public:
   virtual ~Enum() {}

   /**
    * Returns all enumerators the enumeration type has.
    *
    * @param outValues     array which upon return will be filled with enumerators
    */
   virtual void getEnumerators( std::vector< std::string >& outEnumerators ) const {}

   /**
    * Returns an index of the specified enumerator value in the enumerators array.
    *
    * @param enumeratorValue
    */
   virtual int getIndex( int enumeratorValue ) const { return -1; }

   /**
    * Returns a value assigned to the specified enumerator.
    *
    * @param enumeratorName
    */
   virtual int getValue( const std::string& enumeratorName ) const { return -1; }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Serializable enums base class/
 */ 
template< typename DerivedType, typename EnumType >
class EnumBase : public Enum
{
protected:
   typedef std::map< EnumType, std::string > AssocMap;

public:
   /**
    * Constructor.
    */
   explicit EnumBase() {}
   ~EnumBase(){}

   /**
    * Converts from an enumerator to a string.
    *
    * @param enumerator
    * @return              a string representation of an enumerator, 
    *                      or an empty string if the enumerator was not registered.
    */
   static const std::string &from( const EnumType enumerator );

   /**
    * Converts from a string to an enumerator.
    *
    * @param outEnumerator    enumerator
    * @param str              string version of the enumerator
    * @return                 'true' if the conversion is successful, 'false' otherwise.
    */
   static const bool to( const std::string& str, EnumType& outEnumerator );

   /**
    * Returns the map of registered enums.
    */
   static AssocMap &getMap();

   // -------------------------------------------------------------------------
   // Enum implementation
   // -------------------------------------------------------------------------
   void getEnumerators( std::vector< std::string >& outEnumerators ) const;
   int getIndex( int enumeratorValue ) const;
   int getValue( const std::string& enumeratorName ) const;

protected:
   /**
    * Use this helper function to register each enumerator
    * and its string representation.
    *
    * @param enumerator
    * @param eStr          string representation of the enumerator
    */
   static void registerEnumerator( const EnumType enumerator, const std::string &eStr );

private:
   /**
    * Copy constructor.
    */
   EnumBase( const EnumBase& );

   /**
    * Assignment operator.
    */
   const EnumBase& operator=( const EnumBase& );
};

///////////////////////////////////////////////////////////////////////////////

template< typename EnumType > 
struct TEnum : public EnumBase< TEnum< EnumType >, EnumType >
{
private:
   static Class* s_class;

public:
   /**
    * This method needs to be implemented with enumerators definitions.
    */
   static void registerEnumerators();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Enum definition start macro.
 */
#define BEGIN_ENUM( EnumerationName )                                         \
   template <> struct TEnum< EnumerationName > :                              \
   public EnumBase< TEnum< EnumerationName >, EnumerationName >               \
   {                                                                          \
   private:                                                                   \
      static Class* s_class;                                                  \
   public:                                                                    \
      static Class& getRTTIClass() { return *s_class; }                       \
      static void registerEnumerators();                                      \
   };                                                                         \
   void TEnum< EnumerationName >::registerEnumerators() {

///////////////////////////////////////////////////////////////////////////////

/**
 * Single enumerator definition macro.
 */
#define ENUM_VAL( EnumeratorName )                                            \
   registerEnumerator( EnumeratorName, #EnumeratorName );

///////////////////////////////////////////////////////////////////////////////

/**
 * Enum definition end macro.
 */
#define END_ENUM( EnumerationName )                                           \
      ClassTemplate& thisClass =                                              \
         getClassesRegistry().defineClass< typename TEnum< EnumerationName > >(); \
      thisClass.setCreator( new SolidCreator< typename TEnum< EnumerationName > >() ); \
      thisClass.addParent( TypeID< Enum >().name() );                         \
      s_class = new Class( thisClass );                                       \
   }                                                                          \
   Class* TEnum< EnumerationName >::s_class;

///////////////////////////////////////////////////////////////////////////////

/**
 * Enum RTTI registration macro
 */
#ifndef IMPORT_RTTI_REGISTRATIONS
#define REGISTER_ENUM_RTTI( EnumerationName )
#else
#define REGISTER_ENUM_RTTI( EnumerationName )                                 \
   struct EnumerationName##RTTIImporter                                       \
   {                                                                          \
      EnumerationName##RTTIImporter()                                         \
      {                                                                       \
         TEnum< EnumerationName >::getMap();                                  \
      }                                                                       \
   } registerClass##EnumerationName;
#endif

///////////////////////////////////////////////////////////////////////////////

template< typename T >
class TypeID< TEnum< T > >
{
private:
   std::string    m_name;
   unsigned int   m_hash;

public:
   /**
    * Constructor for regular types.
    */
   TypeID();

   /**
    * Returns the name of the type.
    */
   inline const std::string& name() const;

   /**
    * Returns the hash value for the type.
    */
   inline unsigned int hash() const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core/Enum.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _STRING_ENUM_H

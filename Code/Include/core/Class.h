#ifndef _CLASS_H
#define _CLASS_H

/// @file   core\Class.h
/// @file   a definition of an RTTI class

#include "core\ClassesRegistry.h"
#include "core\ClassTemplate.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

class Class;

///////////////////////////////////////////////////////////////////////////////

class RTTIObject
{
public:
   virtual ~RTTIObject() {}

   virtual const Class& getVirtualClass() const = 0;
};


///////////////////////////////////////////////////////////////////////////////

class Class
{
private:
   ClassTemplate*    m_template;

public:
   /**
    * Default constructor.
    */
   Class();

   /**
    * Type based constructor.
    */
   template<typename T>
   static Class createClass()
   {
      ClassTemplate& temp = getClassesRegistry().getClassByType<T>();
      return Class( temp );
   }

   /**
    * Template based constructor.
    *
    * @param temp    class template
    */
   Class( ClassTemplate& temp );

   /**
    * Constructor (name based).
    */
   Class(const std::string& name);

   /**
    * Constructor (handle based).
    */
   Class(unsigned int handle);

   /**
    * Copy constructor.
    */
   Class(const Class& rhs);

   /**
    * Tells whether the class instance describes an existing data type.
    */
   bool isValid() const;

   /**
    * Tells whether the class can be instantiated.
    */
   bool isAbstract() const;

   /**
    * Returns the name of the class type.
    */
   const std::string& getName() const;

   /**
    * Returns a short name of the class, which would be the last 
    * part of its full name.
    *    i.e. ::Namespace::ClassA            ---> ClassA
    *         ::Namespace::ClassA< Class B > ---> ClassA (templates are discarded)
    */
   std::string getShortName() const;

   /**
    * Returns a unique handle assigned to this class type.
    */
   unsigned int getHandle() const;

   /**
    * Checks if this class can be safely cast onto the specified reference type
    * 
    * @param referenceType  type the compatibility with which we want to verify
    */
   bool isA( const Class& referenceType ) const;

   /**
    * Checks if the specified class is of the exactly same type as this class.
    * 
    * @param rhs  class the type of which we want to check
    */
   bool isExactlyA(const Class& rhs) const;

   /**
    * The operator performs an is-exactly-a comparison
    * of the two types.
    */
   bool operator==(const Class& rhs) const;

   /**
    * The operator performs an is-exactly-a comparison
    * of the two types and returns true if the types are different.
    */
   bool operator!=(const Class& rhs) const;

   /**
    * This operator allows to use the classes as keys in hash-maps and hash-sets.
    * Is-exactly-a comparison of the two types is performed.
    */
   bool operator<(const Class& rhs) const;

   /**
    * This method fills the specified collection with handles
    * of the classes that are the parents of the class registered
    * under the specified handle.
    *
    * @throw std::out_of_range   if the class hasn't been registered
    *
    * @param handle              handle to the class parents of whose 
    *                            we want to check
    * @param outParentHandles    collection that will be filled with
    *                            the parents' handles
    */
   void getParents( std::vector<Class>& outParentHandles ) const;

   /**
    * Creates a new instance of the class.
    */
   template<typename T>
   T* instantiate() const
   {
      if ( m_template->isAbstract() )
      {
         throw std::logic_error( "Abstract classes can't be instantiated" );
      }
      void* ptr = m_template->instantiate();
      T* obj = reinterpret_cast<T*> (ptr);

      return obj;
   }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Returns the singleton instance of the registry of classes.
 */
ClassesRegistry& getClassesRegistry();

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro inside a class's declaration body to indicate that 
 * we want it to be a part of the RTTI system.
 */
#define DECLARE_RTTI_CLASS                                                    \
   private:                                                                   \
      static Class* s_class;                                                  \
   public:                                                                    \
      static Class& getRTTIClass() { return *s_class; }                       \
      const Class& getVirtualClass() const { return *s_class; }               \
      static void registerRTTI();                                             \
   private:

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro inside a structure's declaration body to indicate that 
 * we want it to be a part of the RTTI system.
 */
#define DECLARE_RTTI_STRUCT                                                   \
   private:                                                                   \
      static Class* s_class;                                                  \
   public:                                                                    \
      static Class& getRTTIClass() { return *s_class; }                       \
      const Class& getVirtualClass() const { return *s_class; }               \
      static void registerRTTI();

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro to register a class with the reflection system.
 */
#define BEGIN_RTTI(ClassType)                                                 \
   Class* ClassType::s_class;                                                 \
   void ClassType::registerRTTI()                                             \
   {                                                                          \
      ClassTemplate& thisClass =                                              \
         getClassesRegistry().defineClass< ClassType >();                     \
      thisClass.setCreator( new SolidCreator< ClassType >() );                \
      s_class = new Class( thisClass );

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro to register an abstract class with the reflection system.
 */
#define BEGIN_ABSTRACT_RTTI(ClassType)                                        \
   Class* ClassType::s_class;                                                 \
   void ClassType::registerRTTI()                                             \
   {                                                                          \
      ClassTemplate& thisClass =                                              \
         getClassesRegistry().defineClass< ClassType >();                     \
      s_class = new Class( thisClass );

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro ends the RTTI type implementation
 */
#define END_RTTI }

///////////////////////////////////////////////////////////////////////////////

/**
 * RTTI type registration macro.
 */
#ifndef IMPORT_RTTI_REGISTRATIONS
#define REGISTER_RTTI(ClassType)
#else
#define REGISTER_RTTI(ClassType)                                              \
   struct ClassType##RTTIImporter                                             \
   {                                                                          \
      ClassType##RTTIImporter()                                               \
      {                                                                       \
         ClassType::registerRTTI();                                           \
      }                                                                       \
} registerClass##ClassType;
#endif

///////////////////////////////////////////////////////////////////////////////

#define PARENT(ParentClassType)                                               \
   thisClass.addParent( TypeID< ParentClassType >().name() );

///////////////////////////////////////////////////////////////////////////////

#endif // _CLASS_H

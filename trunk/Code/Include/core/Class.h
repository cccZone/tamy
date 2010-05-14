#ifndef _CLASS_H
#define _CLASS_H

/// @file   core\Class.h
/// @file   a definition of an RTTI class

#include "core\ClassesRegistry.h"
#include "core\ClassTemplate.h"


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
    * Returns the name of the class type.
    */
   const std::string& getName() const;

   /**
    * Returns a unique handle assigned to this class type.
    */
   unsigned int getHandle() const;

   /**
    * Checks if the specified class can be safely cast
    * onto this class's type
    * 
    * @param rhs  class the type of which we want to check
    */
   bool isA(const Class& rhs) const;

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
   T* instantiate()
   {
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
      class RegisterMe                                                        \
      {                                                                       \
      public:                                                                 \
         RegisterMe();                                                        \
      };                                                                      \
      static RegisterMe s_classRegistryTool;                                  \
      static Class s_class;                                                   \
   public:                                                                    \
      static Class& getRTTIClass() { return s_class; }                        \
      const Class& getVirtualClass() const { return s_class; }                \
   private:

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro inside a structure's declaration body to indicate that 
 * we want it to be a part of the RTTI system.
 */
#define DECLARE_RTTI_STRUCT                                                   \
   private:                                                                   \
      class RegisterMe                                                        \
      {                                                                       \
      public:                                                                 \
         RegisterMe();                                                        \
      };                                                                      \
      static RegisterMe s_classRegistryTool;                                  \
      static Class s_class;                                                   \
   public:                                                                    \
      static Class& getRTTIClass() { return s_class; }                        \
      const Class& getVirtualClass() const { return s_class; }

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro to register a class with the reflection system.
 */
#define BEGIN_RTTI(ClassType)                                                 \
Class ClassType::s_class;                                                     \
ClassType::RegisterMe ClassType::s_classRegistryTool;                         \
ClassType::RegisterMe::RegisterMe()                                           \
{                                                                             \
   ClassTemplate& thisClass =                                                 \
      getClassesRegistry().defineClass< ClassType >();                        \
   s_class = Class( thisClass );

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro ends the RTTI type implementation
 */
#define END_RTTI }

///////////////////////////////////////////////////////////////////////////////

#define PARENT(ParentClassType)                                               \
   thisClass.addParent( TypeID< ParentClassType >().name() );

///////////////////////////////////////////////////////////////////////////////

#endif // _CLASS_H

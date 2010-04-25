#ifndef _CLASS_H
#define _CLASS_H

/// @file   core\Class.h
/// @file   a definition of an RTTI class

#include "core\ClassesRegistry.h"
#include <typeinfo>


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
   std::string                m_name;
   int                        m_handle;

   std::vector<std::string>   m_parents;

public:
   /**
    * Constructor (default).
    */
   Class();

   /**
    * Class based constructor.
    */
   template<typename T>
   static Class createClass()
   {
      int handle = getClassesRegistry().getHandle<T>();
      return Class(handle);
   }

   /**
    * Constructor (name based).
    */
   Class(const std::string& name);

   /**
    * Constructor (handle based).
    */
   Class(int handle);

   /**
    * Copy constructor.
    */
   Class(const Class& rhs);

   /**
    * Returns the name of the class type.
    */
   inline const std::string& getName() const { return m_name; }

   /**
    * Returns a unique handle assigned to this class type.
    */
   inline int getHandle() const { return m_handle; }

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
   void getParents(std::vector<Class>& outParentHandles) const;

   /**
    * Defines a new parent for the class.
    */
   void addParent(const std::string& classTypeName);

   /**
    * Creates a new instance of the class.
    */
   template<typename T>
   static T* createInstance(const std::string& typeName)
   {
      ClassesRegistry& registry = getClassesRegistry();
      int handle = registry.getHandle(typeName);

      void* ptr = getClassesRegistry().create(handle);
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
      class RegisterClass                                                     \
      {                                                                       \
      public:                                                                 \
         RegisterClass();                                                     \
         void realize() {}                                                    \
      };                                                                      \
      static RegisterClass s_classRegistryTool;                               \
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
      class RegisterClass                                                     \
      {                                                                       \
      public:                                                                 \
         RegisterClass();                                                     \
         void realize() {}                                                    \
      };                                                                      \
      static RegisterClass s_classRegistryTool;                               \
      static Class s_class;                                                   \
   public:                                                                    \
      static Class& getRTTIClass() { return s_class; }                        \
      const Class& getVirtualClass() const { return s_class; }

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro to register a class with the reflection system.
 */
#define BEGIN_ABSTRACT_RTTI(ClassType)                                        \
Class ClassType::s_class;                                                     \
ClassType::RegisterClass ClassType::s_classRegistryTool;                      \
ClassType::RegisterClass::RegisterClass()                                     \
{                                                                             \
   getClassesRegistry().defineAbstract<ClassType>(#ClassType);                \
   Class& thisClass = ClassType::getRTTIClass();                              \
   thisClass = Class(#ClassType);

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro to register a class with the reflection system.
 */
#define BEGIN_RTTI(ClassType)                                                 \
Class ClassType::s_class;                                                     \
ClassType::RegisterClass ClassType::s_classRegistryTool;                      \
ClassType::RegisterClass::RegisterClass()                                     \
{                                                                             \
   getClassesRegistry().defineSolid<ClassType>(#ClassType);                   \
   Class& thisClass = ClassType::getRTTIClass();                              \
   thisClass = Class(#ClassType);

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro to register a template class with the reflection system.
 */
#define BEGIN_TEMPLATE_RTTI(T, ClassType)                                     \
template<typename T>                                                          \
Class ClassType::s_class;                                                     \
template<typename T>                                                          \
typename ClassType::RegisterClass ClassType::s_classRegistryTool;             \
template<typename T>                                                          \
ClassType::RegisterClass::RegisterClass()                                     \
{                                                                             \
   std::string fullName = #ClassType + std::string("<") +                     \
                           typeid(T).name() + std::string(">");               \
   getClassesRegistry().defineSolid< ClassType >(fullName);                   \
   Class& thisClass = ClassType::getRTTIClass();                              \
   thisClass = Class(fullName);

///////////////////////////////////////////////////////////////////////////////

/**
 * Call this macro in the RTTI class's constructors in order to ensure its
 * proper registration with the RTTI system.
 */
#define REALIZE_TEMPLATE_RTTI()                                               \
   s_classRegistryTool.realize();


///////////////////////////////////////////////////////////////////////////////

/**
 * This macro ends the RTTI type implementation
 */
#define END_RTTI }

///////////////////////////////////////////////////////////////////////////////

#define PARENT(ParentClassType)                                               \
   thisClass.addParent(#ParentClassType);

///////////////////////////////////////////////////////////////////////////////

#endif // _CLASS_H

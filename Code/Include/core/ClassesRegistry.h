#ifndef _CLASSES_REGISTRY_H
#define _CLASSES_REGISTRY_H

/// @file   core\ClassesRegistry.h
/// @brief  heart of the reflection mechanism

#include <vector>
#include <map>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class Class;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class is the core of the reflection mechanism, that allows
 * to create classes using arbitrary class ids (such as integer values).
 */
class ClassesRegistry
{
private:
   class ClassCreator
   {
   public:
      virtual ~ClassCreator() {}

      virtual void* create() = 0;
   };

   template <typename T>
   class TClassCreator : public ClassCreator
   {
   public:
      void* create() {return new T();}
   };

   class AbstractClassCreator : public ClassCreator
   {
   public:
      void* create() {return NULL;}
   };
   
   typedef std::map<std::string, int> ClassHandlesMap;

private:
   std::vector<ClassCreator*> m_creators;
   ClassHandlesMap            m_handlesMap;
   ClassHandlesMap            m_typesMap;

public:
   ClassesRegistry();
   ~ClassesRegistry();

   /**
    * This method registers a new abstract class with the registry, assigning it
    * a unique handle using which an instance of the class can be identified.
    *
    * @param ClassType     class type to register

    * @return              handle assigned to this class type
    */
   template <typename ClassType>
   int defineAbstract(const std::string& className);

   /**
    * This method registers a new solid class with the registry, assigning it
    * a unique handle using which an instance of the class can be identified.
    *
    * @param className     name of the class type
    * @return              handle assigned to this class type
    */
   template <typename ClassType>
   int defineSolid(const std::string& className);

   /**
    * This method can be used to obtain a handle of a registered
    * class.
    *
    * @throw std::out_of_range   if the class hasn't been registered
    *
    * @param className  name of the class type
    * @return           handle assigned to this class type
    */
   int getHandle(const std::string& className);

   /**
    * Returns the name of a class to which the specified handle is assigned.
    *
    * @throw std::out_of_range   if the class hasn't been registered
    *
    * @param handle     class handle
    * @return           name of the class
    */
   const std::string& getClassName(int handle) const;

   /**
    * This method can be used to obtain a handle of a registered
    * class.
    *
    * @throw std::out_of_range   if the class hasn't been registered
    *
    * @param ClassType     class type
    *
    * @return              handle assigned to this class type
    */
   template<typename ClassType>
   int getHandle();

   /**
    * This method allows to create an instance of a class based
    * on a unique handle assigned to each class during registration.
    *
    * @param classHandle   handle of a class an instance of which we want to create
    * @return              instance of the class
    */
   void* create(int classHandle);
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ClassesRegistry.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _CLASSES_REGISTRY_H

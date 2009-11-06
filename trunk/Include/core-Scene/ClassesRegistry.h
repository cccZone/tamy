#ifndef _CLASSES_REGISTRY_H
#define _CLASSES_REGISTRY_H

/// @file   core-Scene\ClassesRegistry.h
/// @brief  heart of the reflection mechanism

#include <vector>
#include <map>
#include <string>


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

private:
   std::vector<ClassCreator*> m_creators;

   typedef std::map<std::string, int> ClassHandlesMap;
   ClassHandlesMap m_handlesMap;

public:
   ~ClassesRegistry();

   /**
    * This method registers a new class with the registry, assigning it
    * a unique handle using which an instance of the class can be created.
    *
    * @param ClassType  class type to register
    * @return           handle assigned to this class type
    */
   template <typename ClassType>
   int define();

   /**
    * This method allows to create an instance of a class based
    * on a class handle.
    *
    * @param classHandle   handle of a class an instance of which we want to create
    * @return              instance of the class
    */
   void* create(int classHandle);
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Scene\ClassesRegistry.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _CLASSES_REGISTRY_H

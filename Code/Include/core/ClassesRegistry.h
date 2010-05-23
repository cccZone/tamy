#ifndef _CLASSES_REGISTRY_H
#define _CLASSES_REGISTRY_H

/// @file   core\ClassesRegistry.h
/// @brief  heart of the reflection mechanism

#include <vector>
#include <map>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class ClassTemplate;
class ClassCreator;
class Class;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class is the core of the reflection mechanism, that allows
 * to create classes using arbitrary class ids (such as integer values).
 */
class ClassesRegistry
{
   typedef std::vector< ClassTemplate* >         ClassesArr;
   typedef std::map< std::string, int >          ClassNameMap;
   typedef std::map< unsigned int, int >         ClassHandlesMap;


private:
   ClassesArr                       m_classes;
   ClassNameMap                     m_classNamesMap;
   ClassHandlesMap                  m_classHandlesMap;

public:
   ClassesRegistry();
   ~ClassesRegistry();

   /**
    * This method registers a new abstract class with the registry, assigning it
    * a unique handle using which an instance of the class can be identified.
    *
    * @param ClassType     class type to register
    * @return              created class
    */
   template < typename ClassType >
   ClassTemplate& defineClass();

   /**
    * This method can be used to obtain an instance of a registered
    * class.
    *
    * @throw std::out_of_range   if the class hasn't been registered
    *
    * @param className  name of the class type
    * @return           class instance
    */
   ClassTemplate& getClassByName( const std::string& className );

   /**
    * Returns the name of a class to which the specified handle is assigned.
    *
    * @throw std::out_of_range   if the class hasn't been registered
    *
    * @param handle     class handle
    * @return           class instance
    */
   ClassTemplate& getClassByHandle( unsigned int handle );

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
   template< typename ClassType >
   ClassTemplate& getClassByType();

   /**
    * This method creates a list of all classes that can be substituted
    * dynamically cast to the specified class type.
    *
    * @throw std::out_of_range   if the class hasn't been registered
    *
    * @param ClassType        class type
    * @param includeAbstract  includes abstract classes in the results
    * @param outClasses       results
    */
   template< typename ClassType >
   void getClassesMatchingType( std::vector< Class >& outClasses, bool includeAbstract = false );
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ClassesRegistry.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _CLASSES_REGISTRY_H

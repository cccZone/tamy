/// @file   core/ReflectionLoader.h
/// @brief  serialization tool for loading reflection objects
#ifndef _REFLECTION_LOADER_H
#define _REFLECTION_LOADER_H

#include <vector>
#include <list>
#include "core/types.h"


///////////////////////////////////////////////////////////////////////////////

class InStream;
class ReflectionObject;
class ReflectionObjectsTracker;

///////////////////////////////////////////////////////////////////////////////

/**
 * Serialization tool for loading reflection objects.
 */
class ReflectionLoader
{
private:
   ReflectionObjectsTracker*                 m_instancesTracker;
   InStream&                                 m_inputStream;
   std::list< ReflectionObject* >            m_loadedObjects;

   // temporary dependencies buffer used when initial dependencies are being loaded
   std::vector< ReflectionObject* >          m_dependencies;

public:
   /**
    * Constructor.
    *
    * @param stream     stream to which we want to serialize objects
    */
   ReflectionLoader( InStream& stream );
   /**
    * Constructor.
    *
    * @param stream     stream to which we want to serialize objects
    * @param tracker    instances tracker
    */
   ReflectionLoader( InStream& stream, ReflectionObjectsTracker& tracker );
   ~ReflectionLoader();

   /**
    * Deserializes an object from the stream.
    *
    * @param T    expected object type
    *
    * @return     deserialized object
    */
   template< typename T >
   T* load();

   /**
    * Returns a pointer to a dependency with the specified index.
    *
    * @param   dependencyIdx
    * @return  dependency
    */
   inline ReflectionObject* findDependency( uint dependencyIdx ) const;

private:
   void initialize();
   bool loadDependencies( InStream& stream );
};

///////////////////////////////////////////////////////////////////////////////

#include "core/ReflectionLoader.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_LOADER_H

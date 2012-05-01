/// @file   core/ReflectionSaver.h
/// @brief  serialization tool for saving reflection objects
#ifndef _REFLECTION_SAVER_H
#define _REFLECTION_SAVER_H

#include <vector>
#include "core/types.h"
#include "core/Array.h"


///////////////////////////////////////////////////////////////////////////////

class ReflectionObject;
class OutStream;

///////////////////////////////////////////////////////////////////////////////

/**
 * Serialization tool for saving reflection objects.
 */
class ReflectionSaver
{
private:
   std::vector< const ReflectionObject* >    m_dependencies;
   std::vector< uint >                       m_serializedObjectsIndices;
   OutStream&                                m_outStream;

public:
   /**
    * Constructor.
    *
    * @param stream     stream to which we want to serialize objects
    */
   ReflectionSaver( OutStream& stream );
   ~ReflectionSaver();

   /**
    * Serializes the specified object to the stream.
    * The object has to have an RTTI information.
    *
    * Keep in mind that you can serialize several objects using this method,
    * and if you do that, the pointers they might contain will be mapped
    * to the same objects. This allows you to serialize large number of objects
    * without loosing track of their dependencies.
    *
    * However, the method on its own doesn't flush the data to the output stream
    * specified in the constructor. You either have to call the 'flush' method
    * to do that ( the instance will start remapping the dependencies from the start then )
    * or destroy the instance of the class - destructor flushes the serialized data automatically.
    *
    * @param object
    */
   template< typename T >
   void save( const T& object );

   /**
    * Flushes the serialized data out to the output stream, thus finishing
    * the serialization process.
    */
   void flush();

   /**
    * Adds a dependency and returns its index.
    *
    * @param   dependency
    */
   void addDependency( const ReflectionObject& dependency );

   /**
    * Returns a pointer to a dependency with the specified index.
    *
    * @param   reference to a dependency
    * @return  index of the dependency
    */
   inline uint findDependency( const ReflectionObject& dependency ) const;

};

///////////////////////////////////////////////////////////////////////////////

#include "core/ReflectionSaver.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_SAVER_H

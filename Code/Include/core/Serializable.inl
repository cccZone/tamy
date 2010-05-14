#ifndef _SERIALIZABLE_H
#error "This file can only be included from Serializable.h"
#else

#include "core\Serializer.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

void Serializable::save(Serializer& serializer)
{
   int entityClassID = getClassID();
   serializer << entityClassID;
   onSave(serializer);
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
Derived* Serializable::load(Serializer& serializer)
{
   int entityClassID;
   serializer << entityClassID;

   Class entityClass( entityClassID );
   Derived* object = entityClass.instantiate< Derived >();

   object->onLoad(serializer);
   return object;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _SERIALIZABLE_H

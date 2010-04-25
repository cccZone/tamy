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
   Derived* object = reinterpret_cast<Derived*> (getClassesRegistry().create(entityClassID));

   object->onLoad(serializer);
   return object;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _SERIALIZABLE_H

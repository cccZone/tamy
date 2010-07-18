/// @file   ml-Blender/IResourceInstantiator.h
/// @brief  a resource instantiation interface

#pragma once


///////////////////////////////////////////////////////////////////////////////

class Entity;

///////////////////////////////////////////////////////////////////////////////

/**
 * A resource instantiation interface.
 */
class IResourceInstantiator
{
public:
   virtual ~IResourceInstantiator() {}

   /**
    * Instantiates a resource into an entity that can be hooked
    * up with a scene object.
    */
   virtual Entity* instantiate() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

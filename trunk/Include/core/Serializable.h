#pragma once

/// @file   core\Serializable.h
/// @brief  interface marking things that can be serialized


///////////////////////////////////////////////////////////////////////////////

class Serializer;

///////////////////////////////////////////////////////////////////////////////

class Serializable
{
public:
   virtual ~Serializable() {}

   /**
    * The method allows to save the state of serializable object.
    *
    * @param serializer  serializer that will take care of writing
    *                    stuff to an external storage
    */
   virtual void save(Serializer& serializer) = 0;

   /**
    * The method restores the state of serializable object from 
    * an external storage.
    *
    * @param serializer  serializer that will take care of reading
    *                    stuff from an external storage
    */
   virtual void load(Serializer& serializer) = 0;
};

///////////////////////////////////////////////////////////////////////////////

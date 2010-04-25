#pragma once

/// @file   core\SerializerImpl.h
/// @brief  tool for saving/loading serializables to external storage

#include "core\UniqueObject.h"
#include <string>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Serializable;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class gives the serializables abilities to serialize
 * their state to an external device.
 *
 * Serialization is a transactional process, and a single serializer
 * life cycle can be thought of as a whole transaction - the transaction
 * is opened when it's created, and closed when it's deleted.
 */
class SerializerImpl
{
public:
   virtual ~SerializerImpl() {}

   /**
    * Serializes the specified amount of bytes from the specified data buffer.
    *
    * @param buf           buffer with data to serialize
    * @param bytesCount    amount of data to serialize
    */
   virtual void write(byte* buf, unsigned int bytesCount) = 0;

   /**
    * Deserializes the specified amount of bytes and stores them
    * in the specified data buffer.
    *
    * @param buf           buffer we want to store the data in
    * @param bytesCount    amount of data to deserialize
    *
    * @return              amount of data read
    */
   virtual std::size_t read(byte* buf, unsigned int bytesCount) = 0;
};

///////////////////////////////////////////////////////////////////////////////

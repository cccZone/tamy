#pragma once

/// @file   core\FileSerializer.h
/// @brief  serializer that stores data in binary file archive

#include "core\SerializerImpl.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class File;

///////////////////////////////////////////////////////////////////////////////
/**
 * This serializer will persist data in a simple binary file 
 * archive.
 */
class FileSerializer: public SerializerImpl
{
private:
   File* m_archive;

public:
   /**
    * Constructor.
    *
    * @param archive    binary file archive
    */
   FileSerializer(File* archive);
   ~FileSerializer();

   // -------------------------------------------------------------------------
   // Serializer implementation
   // -------------------------------------------------------------------------
   void write(byte* buf, unsigned int bytesCount);
   std::size_t read(byte* buf, unsigned int bytesCount);
};

///////////////////////////////////////////////////////////////////////////////
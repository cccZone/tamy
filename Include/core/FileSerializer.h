#pragma once

/// @file   core\FileSerializer.h
/// @brief  serializer that stores data in binary file archive

#include "core\Serializer.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class File;

///////////////////////////////////////////////////////////////////////////////

/**
 * This serializer will persist data in a simple binary file 
 * archive.
 */
class FileSerializer : public Serializer
{
private:
   enum Marker
   {
      MARK_STRING = 1,
      MARK_MATRIX,
      MARK_VEC3
   };

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
   // ModelSerializer implementation
   // -------------------------------------------------------------------------
   void saveInt(int val);
   int loadInt();
   void saveFloat(float val);
   float loadFloat();
   void saveBool(bool val);
   bool loadBool();
   void saveChar(char val);
   char loadChar();
   void saveLong(long val);
   long loadLong();
   void saveString(const std::string& str);
   void loadString(std::string& str);
   void saveMatrix(const D3DXMATRIX& mtx);
   void loadMatrix(D3DXMATRIX& mtx);
   void saveVec3(const D3DXVECTOR3& vec);
   void loadVec3(D3DXVECTOR3& vec);

private:
   void writeMarker(Marker marker);
   void confirmMarker(Marker marker);
};

///////////////////////////////////////////////////////////////////////////////

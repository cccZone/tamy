#include "core\FileSerializer.h"
#include "core\File.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

FileSerializer::FileSerializer(File* archive)
: m_archive(archive)
{
   if (m_archive == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a File instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

FileSerializer::~FileSerializer()
{
   delete m_archive;
   m_archive = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::saveInt(int val)
{
   m_archive->write((byte*)&val, sizeof(int));
}

///////////////////////////////////////////////////////////////////////////////

int FileSerializer::loadInt()
{
   int val = 0;
   std::size_t bytesRead = m_archive->read((byte*)&val, sizeof(int));
   if (bytesRead != sizeof(int))
   {
      throw std::runtime_error("Error reading a binary archive");
   }

   return val;
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::saveFloat(float val)
{
   m_archive->write((byte*)&val, sizeof(float));
}

///////////////////////////////////////////////////////////////////////////////

float FileSerializer::loadFloat()
{
   float val = 0;
   std::size_t bytesRead = m_archive->read((byte*)&val, sizeof(float));
   if (bytesRead != sizeof(float))
   {
      throw std::runtime_error("Error reading a binary archive");
   }

   return val;
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::saveBool(bool val) 
{
   m_archive->write((byte*)&val, sizeof(bool));
}

///////////////////////////////////////////////////////////////////////////////

bool FileSerializer::loadBool() 
{
   bool val = 0;
   std::size_t bytesRead = m_archive->read((byte*)&val, sizeof(bool));
   if (bytesRead != sizeof(bool))
   {
      throw std::runtime_error("Error reading a binary archive");
   }

   return val;
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::saveChar(char val) 
{
   m_archive->write((byte*)&val, sizeof(char));
}

///////////////////////////////////////////////////////////////////////////////

char FileSerializer::loadChar() 
{
   char val = 0;
   std::size_t bytesRead = m_archive->read((byte*)&val, sizeof(char));
   if (bytesRead != sizeof(char))
   {
      throw std::runtime_error("Error reading a binary archive");
   }

   return val;
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::saveLong(long val) 
{
   m_archive->write((byte*)&val, sizeof(long));
}

///////////////////////////////////////////////////////////////////////////////

long FileSerializer::loadLong()
{
   long val = 0;
   std::size_t bytesRead = m_archive->read((byte*)&val, sizeof(long));
   if (bytesRead != sizeof(long))
   {
      throw std::runtime_error("Error reading a binary archive");
   }

   return val;
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::saveString(const std::string& str)
{
   writeMarker(MARK_STRING);

   unsigned int strLen = str.length();
   m_archive->write((byte*)&strLen, sizeof(unsigned int));
   m_archive->write((byte*)str.c_str(), sizeof(char) * strLen);
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::loadString(std::string& str)
{
   confirmMarker(MARK_STRING);

   unsigned int strLen = 0;
   m_archive->read((byte*)&strLen, sizeof(unsigned int));
   
   char* strBuf = new char[strLen + 1];
   m_archive->read((byte*)strBuf, sizeof(char) * strLen);
   strBuf[strLen] = 0;
   str = strBuf;
   delete [] strBuf;
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::saveMatrix(const D3DXMATRIX& mtx)
{
   writeMarker(MARK_MATRIX);
   m_archive->write((byte*)&(mtx.m), sizeof(float) * 16);
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::loadMatrix(D3DXMATRIX& mtx)
{
   confirmMarker(MARK_MATRIX);
   std::size_t bytesToRead = sizeof(float) * 16;
   std::size_t bytesRead = m_archive->read((byte*)&(mtx.m), bytesToRead);
   if (bytesRead != bytesToRead)
   {
      throw std::runtime_error("Error reading a binary archive");
   }
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::saveVec3(const D3DXVECTOR3& vec)
{
   writeMarker(MARK_VEC3);
   m_archive->write((byte*)&(vec.x), sizeof(float));
   m_archive->write((byte*)&(vec.y), sizeof(float));
   m_archive->write((byte*)&(vec.z), sizeof(float));
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::loadVec3(D3DXVECTOR3& vec)
{
   confirmMarker(MARK_VEC3);
   m_archive->read((byte*)&(vec.x), sizeof(float));
   m_archive->read((byte*)&(vec.y), sizeof(float));
   m_archive->read((byte*)&(vec.z), sizeof(float));
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::writeMarker(Marker marker)
{
   m_archive->write((byte*)&marker, sizeof(byte));
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::confirmMarker(Marker marker)
{
   byte markerFromFile = 0;
   m_archive->read(&markerFromFile, sizeof(byte));

   if (markerFromFile != marker)
   {
      throw std::runtime_error("Serializer expected to find a different primitive here");
   }
}

///////////////////////////////////////////////////////////////////////////////

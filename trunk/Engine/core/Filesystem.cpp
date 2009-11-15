#include "core\Filesystem.h"
#include "core\File.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

void Filesystem::changeRootDir(const std::string& rootDir)
{
   m_rootDir = rootDir;

   std::size_t endingSlashPos = m_rootDir.find_last_of("/\\");
   if (endingSlashPos != (m_rootDir.length() - 1))
   {
      m_rootDir += "/";
   }
}

///////////////////////////////////////////////////////////////////////////////

const std::string& Filesystem::getCurrRoot() const
{
   return m_rootDir;
}

///////////////////////////////////////////////////////////////////////////////

bool Filesystem::doesExist(const std::string& fileName) const
{
   File* file = NULL;
   try
   {
      file = open(fileName);
   }
   catch (std::runtime_error&)
   {
      file = NULL;
   }

   bool exists = (file != NULL);
   delete file;

   return exists;
}

///////////////////////////////////////////////////////////////////////////////

File* Filesystem::open(const std::string& fileName, 
                       const std::ios_base::openmode mode) const
{
   std::string fullFileName = m_rootDir + fileName;

   return new File(fullFileName, mode);
}

///////////////////////////////////////////////////////////////////////////////

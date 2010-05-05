#include "core\Filesystem.h"
#include "core\File.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

Filesystem::Filesystem(const std::string& rootDir)
{
   changeRootDir(rootDir);
}

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

std::string Filesystem::extractExtension( const std::string& fileName )
{
   std::size_t pos = fileName.find_last_of( "." );
   if ( pos != std::string::npos )
   {
      return fileName.substr( pos + 1 );
   }
   else
   {
      return "";
   }
}

///////////////////////////////////////////////////////////////////////////////

std::string Filesystem::changeFileExtension( const std::string& fileName, 
                                             const std::string& newExtension )
{
   std::size_t pos = fileName.find_last_of( "." );
   if ( pos != std::string::npos )
   {
      return fileName.substr( 0, pos ) + newExtension;
   }
   else
   {
      return fileName + "." + newExtension;
   }
}

///////////////////////////////////////////////////////////////////////////////

std::string Filesystem::toRelativePath(const std::string& absoluteFilePath) const
{
   // tokenize both the filename and the root dir
   std::vector<std::string> fileNameTokens;
   std::vector<std::string> rootDirTokens;

   tokenize(absoluteFilePath, fileNameTokens, "\\/");
   tokenize(m_rootDir, rootDirTokens, "\\/");

   // if the root dir path contains more or equal amount of nodes
   // as the filename, file can't possibly be located inside that directory
   if (rootDirTokens.size() >= fileNameTokens.size())
   {
      return "";
   }

   // compare all the nodes from the root directory to those on the file
   // to make sure the file is indeed located inside the root director
   unsigned int rootDirTokensCount = rootDirTokens.size();
   for (unsigned int i = 0; i < rootDirTokensCount; ++i)
   {
      if (rootDirTokens[i] != fileNameTokens[i])
      {
         return "";
      }
   }

   // assemble a file name relative to the root directory
   std::string relativeFileName = ".";
   unsigned int fileNameTokensCount = fileNameTokens.size();
   for (unsigned int i = rootDirTokensCount; i < fileNameTokensCount; ++i)
   {
      relativeFileName += "/" + fileNameTokens[i];
   }
   return relativeFileName;
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::tokenize(const std::string& str, 
                          std::vector<std::string>& output,
                          const std::string& tokens) const
{
   std::size_t start, end;
   std::string tmp;
   start = 0;
   end = 0;

   while (end != std::string::npos)
   {
      end = str.find_first_of(tokens, start);
      tmp = str.substr(start, end - start);
      if (tmp.size() > 0)
      {
         output.push_back(tmp);
      }
      start = end + 1;
   }
}

///////////////////////////////////////////////////////////////////////////////

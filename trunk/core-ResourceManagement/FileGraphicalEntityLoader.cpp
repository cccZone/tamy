#include "core-ResourceManagement\FileGraphicalEntityLoader.h"


///////////////////////////////////////////////////////////////////////////////

std::string FileGraphicalEntityLoader::getExtension(const std::string& fileName) const
{
   size_t extMarkerPos = fileName.find_last_of(".");
   if (extMarkerPos == std::string::npos) {return std::string("");}

   return fileName.substr(extMarkerPos + 1);
}

///////////////////////////////////////////////////////////////////////////////

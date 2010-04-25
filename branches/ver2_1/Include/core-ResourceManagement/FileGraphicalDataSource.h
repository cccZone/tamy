#pragma once

#include "core-ResourceManagement\GraphicalDataSource.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a special case of a loader, dedicated to loading
 * meshes from files.
 */
class FileGraphicalDataSource : public GraphicalDataSource
{
public:
   virtual ~FileGraphicalDataSource() {}

   /**
    * The method should return true if it's capable of 
    * loading the specified file.
    */
   virtual bool canHandleFile(const std::string& fileName) const = 0;

protected:
   /**
    * A helper method for extracting extensions from the file names.
    * Let's face it - most of the time we're gonna be depending
    * on the file's extension to tell if it's in the format we can handle,
    * and this method's gonna be very helpful.
    */
   std::string getExtension(const std::string& fileName) const;
};

///////////////////////////////////////////////////////////////////////////////

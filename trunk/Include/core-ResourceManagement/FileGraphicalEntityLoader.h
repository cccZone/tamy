#pragma once

#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a special case of a loader, dedicated to loading
 * meshes from files.
 * ResourceManager stest the context for this class instance call,
 * and it requires for it to implement an additional method
 * that helps it choose the best suited loader for a particualr file.
 */
class FileGraphicalEntityLoader : public GraphicalEntityLoader
{
protected:
   std::string m_meshesDir;

public:
   virtual ~FileGraphicalEntityLoader() {}

   void setMeshesDir(const std::string& dir) {m_meshesDir = dir;}
   const std::string& getMeshesDir() const {return m_meshesDir;}

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

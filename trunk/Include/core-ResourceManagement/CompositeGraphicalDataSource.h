#pragma once

#include "core-ResourceManagement\GraphicalDataSource.h"
#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class FileGraphicalDataSource;

///////////////////////////////////////////////////////////////////////////////

/**
 * This composite will try loading a mesh from a file using 
 * the registered data sources
 */
class CompositeGraphicalDataSource : public GraphicalDataSource
{
private:
   std::string m_meshesDir;
   std::vector<FileGraphicalDataSource*> m_sources;

public:
   CompositeGraphicalDataSource(const std::string& dir);
   ~CompositeGraphicalDataSource();

   void addSource(FileGraphicalDataSource* source);

   void parseMesh(MeshDefinition& mesh, 
                  AnimationDefinition& animation, 
                  std::vector<MaterialDefinition>& materials,
                  const std::string& name);
};

///////////////////////////////////////////////////////////////////////////////

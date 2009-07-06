#include "core-ResourceManagement\CompositeGraphicalDataSource.h"
#include "core-ResourceManagement\FileGraphicalDataSource.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

CompositeGraphicalDataSource::CompositeGraphicalDataSource(const std::string& dir)
      : m_meshesDir(dir)
{
}

///////////////////////////////////////////////////////////////////////////////

CompositeGraphicalDataSource::~CompositeGraphicalDataSource()
{
   unsigned int count = m_sources.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_sources[i];
   }
   m_sources.clear();
}

///////////////////////////////////////////////////////////////////////////////

void CompositeGraphicalDataSource::addSource(FileGraphicalDataSource* source)
{
   if (source == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a FileGraphicalDataSource instance");
   }

   m_sources.push_back(source);
}

///////////////////////////////////////////////////////////////////////////////

void CompositeGraphicalDataSource::parseMesh(MeshDefinition& mesh, 
                                             AnimationDefinition& animation, 
                                             std::vector<MaterialDefinition>& materials,
                                             const std::string& name)
{
   unsigned int count = m_sources.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      FileGraphicalDataSource* source = m_sources[i];
      if (source->canHandleFile(name) == true)
      {
         source->parseMesh(mesh, animation, materials, m_meshesDir + std::string("\\") + name);
         return;
      }
   }

   throw std::runtime_error(std::string("No loader could handle loading file ") + name);
}

///////////////////////////////////////////////////////////////////////////////

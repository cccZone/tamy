#pragma once

#include "core-ResourceManagement\GraphicalEntityLoader.h"


//////////////////////////////////////////////////////////////////////////////

class GraphicalEntityLoaderMock : public GraphicalEntityLoader
{
private:
   MeshDefinition& m_mesh;

public:
   GraphicalEntityLoaderMock(MeshDefinition& mesh) 
         : m_mesh(mesh)
   {}

protected:
   void parseMesh(MeshDefinition& mesh, 
                  AnimationDefinition& anim, 
                  const std::string& name)
   {
      mesh = m_mesh;
   }
};

//////////////////////////////////////////////////////////////////////////////

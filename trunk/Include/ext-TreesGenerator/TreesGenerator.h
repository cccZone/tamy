#pragma once

#include "core-ResourceManagement\GraphicalDataSource.h"
#include "core-ResourceManagement\MeshDefinition.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

struct TreeParams;

///////////////////////////////////////////////////////////////////////////////

class TreesGenerator : public GraphicalDataSource
{
private:
   MeshDefinition* m_mesh; 
   AnimationDefinition* m_animation;

public:
   TreesGenerator(TreeParams& params,
                  unsigned int skinningResolution,
                  unsigned int bonesResolution,
                  const D3DXVECTOR3& windDirection,
                  float windStrength,
                  float animationLengthInSecs,
                  const std::string& materialName);
   ~TreesGenerator();

   void parseMesh(MeshDefinition& mesh, 
                  AnimationDefinition& animation, 
                  std::vector<MaterialDefinition>& materials,
                  const std::string& name);
};

///////////////////////////////////////////////////////////////////////////////

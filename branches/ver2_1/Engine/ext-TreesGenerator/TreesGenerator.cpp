#include "ext-TreesGenerator\TreesGenerator.h"
#include "ext-TreesGenerator\TreeParams.h"
#include "ext-TreesGenerator\TreeStructureGenerator.h"
#include "ext-TreesGenerator\TreeSkinner.h"
#include "ext-TreesGenerator\TreeSegment.h"
#include "ext-TreesGenerator\TreeAnimator.h"


///////////////////////////////////////////////////////////////////////////////

TreesGenerator::TreesGenerator(TreeParams& params,
                               unsigned int skinningResolution,
                               unsigned int bonesResolution,
                               const D3DXVECTOR3& windDirection,
                               float windStrength,
                               float animationLengthInSecs,
                               const std::string& materialName)
{
   TreeStructureGenerator generator;
   TreeSegment* treeStruct = generator.generate(params);

   TreeSkinner skinner(*treeStruct);
   m_mesh = skinner("tree_template", skinningResolution, bonesResolution, materialName);
   delete treeStruct;

   TreeAnimator animator;
   m_animation = new AnimationDefinition();
   animator(*m_mesh, windDirection, windStrength, animationLengthInSecs, *m_animation);
}

///////////////////////////////////////////////////////////////////////////////

TreesGenerator::~TreesGenerator()
{
   delete m_mesh;
   m_mesh = NULL;

   delete m_animation;
   m_animation = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TreesGenerator::parseMesh(MeshDefinition& mesh, 
                               AnimationDefinition& animation, 
                               std::vector<MaterialDefinition>& materials,
                               const std::string& name)
{
   mesh = *m_mesh;
   animation = *m_animation;
}

///////////////////////////////////////////////////////////////////////////////

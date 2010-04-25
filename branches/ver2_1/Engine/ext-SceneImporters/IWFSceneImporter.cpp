#include "ext-SceneImporters\IWFSceneImporter.h"
#include "core-ResourceManagement\MeshHierarchyFlattener.h"
#include "core-ResourceManagement\MeshDefinition.h"
#include "core-Scene\Model.h"
#include "core-Scene\PointLightEntity.h"
#include "core-Scene\SpotLightEntity.h"
#include "core-Scene\DirectionalLightEntity.h"
#include "core-Scene\SkyBoxEntity.h"
#include "core-Scene\StaticGeometryEntity.h"
#include "core-Scene\MeshDesc.h"
#include "core-Scene\MaterialEntity.h"
#include "core-Scene\AnimatedCutsceneEntity.h"
#include "core-Scene\WorldEntity.h"


///////////////////////////////////////////////////////////////////////////////

IWFSceneImporter::IWFSceneImporter(Model& scene)
: m_scene(scene)
{
}

///////////////////////////////////////////////////////////////////////////////

void IWFSceneImporter::addLight(const std::string& name,
                                const Color& ambient, 
                                const Color& diffuse, 
                                const Color& specular,
                                IWFLightType lightType,
                                float range,
                                float constAtt, float linearAtt, float quadAtt,
                                float fallOff, float theta, float phi,
                                const D3DXMATRIX& situation)
{
   D3DXVECTOR3 pos(situation._41, situation._42, situation._43);

   switch(lightType)
   {
   case LT_POINT:
      {
         m_entities.push_back(new PointLightEntity(name, 
                                          ambient, diffuse, specular, 
                                          range, constAtt, linearAtt, quadAtt, 
                                          pos));
         break;
      }

   case LT_SPOT:
      {
         m_entities.push_back(new SpotLightEntity(name, 
                                         ambient, diffuse, specular, 
                                         range, constAtt, linearAtt, quadAtt, 
                                         fallOff, theta, phi,
                                         situation));
         break;
      }

   case LT_DIRECTIONAL: // fall through
   default:
      {
         m_entities.push_back(new DirectionalLightEntity(name, 
                                                ambient, diffuse, specular, 
                                                situation));
         break;
      }
   }

   // include light's position in world's dimensions
   m_worldDimensions.include(pos);
}

///////////////////////////////////////////////////////////////////////////////

void IWFSceneImporter::addSkyBox(const std::string& frontTex,
                                 const std::string& backTex,
                                 const std::string& leftTex,
                                 const std::string& rightTex,
                                 const std::string& topTex,
                                 const std::string& bottomTex)
{
   m_entities.push_back(new SkyBoxEntity(frontTex, backTex, leftTex, 
                                rightTex, topTex, bottomTex));
}

///////////////////////////////////////////////////////////////////////////////

void IWFSceneImporter::addDynamicMesh(const std::string& meshFileName, 
                                      const D3DXMATRIX& situation)
{
   m_entities.push_back(new AnimatedCutsceneEntity(meshFileName, "unnamedAnim0", situation));

   // include object's position in world's dimensions
   D3DXVECTOR3 pos(situation._41, situation._42, situation._43);
   m_worldDimensions.include(pos);
}

///////////////////////////////////////////////////////////////////////////////

void IWFSceneImporter::addStaticGeometry(MeshDefinition& mesh, 
                                         std::vector<MaterialDefinition>& materials,
                                         const D3DXMATRIX& situation)

{
   // extract and convert materials
   unsigned int count = materials.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      MaterialEntity* materialEntity = new MaterialEntity();

      adaptMaterial(materials[i], *materialEntity);
      m_entities.push_back(materialEntity);
   }

   // extract and convert meshes
   std::vector<FlatMesh*> flatMeshes;
   MeshHierarchyFlattener meshExtractor;
   meshExtractor(mesh, flatMeshes);

   char tmpMeshName[128];
   std::vector<MeshDesc*> meshDescs;
   unsigned int meshesCount = flatMeshes.size();
   for (unsigned int i = 0; i < meshesCount; ++i)
   {
      sprintf_s(tmpMeshName, 128, "staticMesh_%s_%d", mesh.name.c_str(), i);

      MeshDesc* newMesh = new MeshDesc();
      newMesh->m_name = tmpMeshName;
      newMesh->m_isSkin = false;
      newMesh->m_vertices = flatMeshes[i]->vertices;
      newMesh->m_faces = flatMeshes[i]->faces;
      newMesh->m_materials.push_back(flatMeshes[i]->material);

      meshDescs.push_back(newMesh);
   }
   m_entities.push_back(new StaticGeometryEntity(mesh.name.c_str(), meshDescs, situation));


   // include meshes in world's dimensions
   D3DXVECTOR3 vtxWorldPos;
   for (unsigned int meshIdx = 0; meshIdx < meshesCount; ++meshIdx)
   {
      const std::vector<LitVertex>& vertices = meshDescs[meshIdx]->m_vertices;
      unsigned int verticesCount = vertices.size();
      for (unsigned int vtxIdx = 0; vtxIdx < verticesCount; ++vtxIdx)
      {
         D3DXVec3TransformCoord(&vtxWorldPos, &(vertices[vtxIdx].m_coords), &situation);
         m_worldDimensions.include(vtxWorldPos);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void IWFSceneImporter::adaptMesh(MeshDefinition& inMesh, MeshDesc& outMesh)
{
   outMesh.m_name = inMesh.name;
   outMesh.m_isSkin = inMesh.isSkin;
   outMesh.m_materials = inMesh.materials;
   outMesh.m_vertices = inMesh.vertices;
   outMesh.m_faces = inMesh.faces;
   outMesh.m_bonesInfluencingAttribute = inMesh.bonesInfluencingAttribute;
   outMesh.m_skinBones = inMesh.skinBones;
   outMesh.m_localMtx = inMesh.localMtx;

   for (std::list<MeshDefinition*>::iterator it = inMesh.children.begin();
        it != inMesh.children.end(); ++it)
   {
      MeshDesc* childMesh = new MeshDesc();
      outMesh.m_children.push_back(childMesh);
      childMesh->m_parent = &outMesh;
      adaptMesh(**it, *childMesh);
   }
}

///////////////////////////////////////////////////////////////////////////////

void IWFSceneImporter::adaptMaterial(MaterialDefinition& inMat, 
                                     MaterialEntity& outMat)
{
   outMat.m_matName = inMat.matName;
   outMat.m_ambient = inMat.ambient;
   outMat.m_diffuse = inMat.diffuse;
   outMat.m_specular = inMat.specular;
   outMat.m_emissive = inMat.emissive;
   outMat.m_power = inMat.power;
   outMat.m_texName = inMat.texName;
   outMat.m_colorOp = inMat.colorOp;
   outMat.m_colorArg1 = inMat.colorArg1;
   outMat.m_colorArg2 = inMat.colorArg2;
   outMat.m_alphaOp = inMat.alphaOp;
   outMat.m_alphaArg1 = inMat.alphaArg1;
   outMat.m_alphaArg2 = inMat.alphaArg2;
   outMat.m_coordsOp = inMat.coordsOp;
}

///////////////////////////////////////////////////////////////////////////////

void IWFSceneImporter::onFinish()
{
   // add an entity with the world dimensions
   m_scene.add(new WorldEntity(m_worldDimensions));
   m_worldDimensions = AABoundingBox();

   // add all created entities
   for (Entities::iterator it = m_entities.begin();
        it != m_entities.end(); ++it)
   {
      m_scene.add(*it);
   }
   m_entities.clear();
}

///////////////////////////////////////////////////////////////////////////////

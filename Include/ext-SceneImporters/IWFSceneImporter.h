#pragma once

/// @file   ext-SceneImporters\IWFSceneImporter.h
/// @brief  tool for importing scenes stored in IWF files

#include <string>
#include "core-ResourceManagement\IWFScene.h"
#include "core\AABoundingBox.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

class Model;
class Entity;
struct MeshDesc;
struct MaterialEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class will import a scene stored in IWF file into a Tamy scene model.
 */
class IWFSceneImporter : public IWFScene
{
private:
   Model& m_scene;

   AABoundingBox m_worldDimensions;

   typedef std::list<Entity*> Entities;
   Entities m_entities;

public:
   /**
    * Constructor.
    *
    * @param scene      this model is where the imported scene will be 
    *                   placed
    */
   IWFSceneImporter(Model& scene);

   // -------------------------------------------------------------------------
   // IWFScene implementation
   // -------------------------------------------------------------------------
   void addLight(const std::string& name,
                 const Color& ambient, 
                 const Color& diffuse, 
                 const Color& specular,
                 IWFLightType lightType,
                 float range,
                 float constAtt, float linearAtt, float quadAtt,
                 float fallOff, float theta, float phi,
                 const D3DXMATRIX& situation);

   void addSkyBox(const std::string& frontTex,
                  const std::string& backTex,
                  const std::string& leftTex,
                  const std::string& rightTex,
                  const std::string& topTex,
                  const std::string& bottomTex);

   void addDynamicMesh(const std::string& meshFileName, 
                       const D3DXMATRIX& situation);

   void addStaticGeometry(MeshDefinition& mesh, 
                          std::vector<MaterialDefinition>& materials,
                          const D3DXMATRIX& situation);

   void onFinish();

private:
   void adaptMesh(MeshDefinition& inMesh, MeshDesc& outMesh);
   void adaptMaterial(MaterialDefinition& inMat, MaterialEntity& outMat);
};

///////////////////////////////////////////////////////////////////////////////

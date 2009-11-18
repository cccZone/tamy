#pragma once

/// @file   ext-Demo\DemoIWFScene.h
/// @brief  and implementation of IWFScene

#include "core-ResourceManagement\IWFScene.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core\ResourceStorage.h"
#include "core\Delegate.h"
#include "core\StaticGeometryOctree.h"


///////////////////////////////////////////////////////////////////////////////

class Node;
class GraphicalEntitiesFactory;
class GraphicalDataSource;
class Material;
class Texture;
class GraphicalEntityLoader;
class StaticGeometryLoader;
class Light;
class SkyBoxStorage;
class StaticGeometryRenderable;

///////////////////////////////////////////////////////////////////////////////

namespace demo
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This delegate should add lights to a proper scene manager
 */
typedef Delegate<void (Light*)> LightsSceneSetter;

/**
 * This delegate should set the specified skybox so that it's rendered along
 * with the rest of the scene.
 */
typedef Delegate<void (SkyBoxStorage*)> SkyBoxSceneSetter;

/**
 * This delegate should add a new dynamic mesh to the scene.
 * Such a mesh can constist of Nodes and AbstractGraphicalNodes -
 * the scene should recognize that.
 */
typedef Delegate<void (Node*)> DynamicObjectsSceneSetter;

/**
 * This delegate should add a new static mesh to the scene.
 */
typedef Delegate<SGHandle (StaticGeometryRenderable*)> StaticGeometrySetter;

///////////////////////////////////////////////////////////////////////////////

/**
 * An implementation of the IWFScene interface  for the demo purposes
 */ 
class DemoIWFScene : public IWFScene
{
private:
   GraphicalEntitiesFactory& m_entitiesFactory;
   GraphicalDataSource& m_externalEntitiesSource;
   LightsSceneSetter& m_addLight;
   SkyBoxSceneSetter& m_setSkyBox;
   DynamicObjectsSceneSetter& m_addDynamicObj;
   StaticGeometrySetter& m_addStaticObj;
   ResourceStorage<AbstractGraphicalEntity>& m_entitiesStorage;
   ResourceStorage<Material>& m_materialsStorage;

   GraphicalEntityLoader* m_entityLoader;
   StaticGeometryLoader* m_staticGeometryLoader;

public:
   DemoIWFScene(GraphicalEntitiesFactory& entitiesFactory,
                GraphicalDataSource& externalEntitiesSource,
                LightsSceneSetter& addLight,
                SkyBoxSceneSetter& setSkyBox,
                DynamicObjectsSceneSetter& addDynamicObj,
                StaticGeometrySetter& addStaticObj,
                ResourceStorage<AbstractGraphicalEntity>& entitiesStorage,
                ResourceStorage<Material>& materialsStorage);

   ~DemoIWFScene();

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

private:
   Texture* createSkyboxMaterial(const std::string& textureName) const;
};

///////////////////////////////////////////////////////////////////////////////

} // namespace demo

///////////////////////////////////////////////////////////////////////////////
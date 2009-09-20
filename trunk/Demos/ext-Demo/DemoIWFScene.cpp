#include "ext-Demo\DemoIWFScene.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\SkyBoxSide.h"
#include "core-Renderer\SkyBoxStorage.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "core-ResourceManagement\StaticGeometryLoader.h"
#include "core-ResourceManagement\GraphicalDataSource.h"
#include "core-ResourceManagement\StaticGeometry.h"
#include <algorithm>
#include <sstream>


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

DemoIWFScene::DemoIWFScene(GraphicalEntitiesFactory& entitiesFactory,
                           GraphicalDataSource& externalEntitiesSource,
                           LightsSceneSetter& addLight,
                           SkyBoxSceneSetter& setSkyBox,
                           DynamicObjectsSceneSetter& addDynamicObj,
                           StaticGeometrySetter& addStaticObj,
                           ResourceStorage<AbstractGraphicalEntity>& entitiesStorage,
                           ResourceStorage<Material>& materialsStorage)
: m_entitiesFactory(entitiesFactory)
, m_externalEntitiesSource(externalEntitiesSource)
, m_addLight(addLight)
, m_setSkyBox(setSkyBox)
, m_addDynamicObj(addDynamicObj)
, m_addStaticObj(addStaticObj)
, m_entitiesStorage(entitiesStorage)
, m_materialsStorage(materialsStorage)
, m_entityLoader(new GraphicalEntityLoader(entitiesFactory, materialsStorage))
, m_staticGeometryLoader(new StaticGeometryLoader(entitiesFactory))
{
}

///////////////////////////////////////////////////////////////////////////////

DemoIWFScene::~DemoIWFScene()
{
   delete m_entityLoader;
   m_entityLoader = NULL;

   delete m_staticGeometryLoader;
   m_staticGeometryLoader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DemoIWFScene::addLight(const std::string& name,
                            const Color& ambient, 
                            const Color& diffuse, 
                            const Color& specular,
                            IWFLightType lightType,
                            float range,
                            float constAtt, float linearAtt, float quadAtt,
                            float fallOff, float theta, float phi,
                            const D3DXMATRIX& situation)
{
   Light* light = new Light(name);
   light->setAmbientColor(ambient);
   light->setDiffuseColor(diffuse);
   light->setSpecularColor(specular);

   switch(lightType)
   {
   case LT_POINT:       light->setType(Light::LT_POINT);       break;
   case LT_SPOT:        light->setType(Light::LT_SPOT);        break;
   case LT_DIRECTIONAL: light->setType(Light::LT_DIRECTIONAL); break;
   default:             light->setType(Light::LT_POINT);       break;
   }

   light->setRange(range);
   light->setAttenuation(constAtt, linearAtt, quadAtt);
   light->setSpotlightFeatures(fallOff, theta, phi);
   light->setLocalMtx(situation);

   m_addLight(light);
}

///////////////////////////////////////////////////////////////////////////////

void DemoIWFScene::addSkyBox(const std::string& frontTex,
                             const std::string& backTex,
                             const std::string& leftTex,
                             const std::string& rightTex,
                             const std::string& topTex,
                             const std::string& bottomTex)
{
   SkyBoxSide* left     = m_entitiesFactory.createSkyBoxSide(SBS_LEFT, createSkyboxMaterial(leftTex));
   SkyBoxSide* right    = m_entitiesFactory.createSkyBoxSide(SBS_RIGHT, createSkyboxMaterial(rightTex));
   SkyBoxSide* top      = m_entitiesFactory.createSkyBoxSide(SBS_TOP, createSkyboxMaterial(topTex));
   SkyBoxSide* bottom   = m_entitiesFactory.createSkyBoxSide(SBS_BOTTOM, createSkyboxMaterial(bottomTex));
   SkyBoxSide* front    = m_entitiesFactory.createSkyBoxSide(SBS_FRONT, createSkyboxMaterial(frontTex));
   SkyBoxSide* back     = m_entitiesFactory.createSkyBoxSide(SBS_BACK, createSkyboxMaterial(backTex));

   m_setSkyBox(new SkyBoxStorage(left, right, top, bottom, front, back));
}

///////////////////////////////////////////////////////////////////////////////

Texture* DemoIWFScene::createSkyboxMaterial(const std::string& textureName) const
{
   Texture* texture = m_entitiesFactory.createTexture(textureName);
   return texture;
}

///////////////////////////////////////////////////////////////////////////////

void DemoIWFScene::addDynamicMesh(const std::string& meshFileName, 
                                  const D3DXMATRIX& situation)
{
   AbstractGraphicalEntity* entity = NULL;
   if (m_entitiesStorage.is(meshFileName))
   {
      entity = &(m_entitiesStorage.get(meshFileName));
   }
   else
   {
      entity = m_entityLoader->load(meshFileName, m_externalEntitiesSource);
      m_entitiesStorage.add(entity);
   }
   
   static int refCount = 0;
   std::stringstream refName;
   refName << "reference_" << refCount++;
   GraphicalEntityInstantiator* entityInstance = new GraphicalEntityInstantiator(refName.str());
   entityInstance->attachEntity(*entity);
   entityInstance->setLocalMtx(situation);

   m_addDynamicObj(entityInstance);
}

///////////////////////////////////////////////////////////////////////////////

void DemoIWFScene::addStaticGeometry(MeshDefinition& mesh, 
                                     std::vector<MaterialDefinition>& materials,
                                     const D3DXMATRIX& situation)
{
   class ExistingDefSource : public GraphicalDataSource
   {
   private:
      MeshDefinition& m_mesh;
      std::vector<MaterialDefinition>& m_materials;

   public:
      ExistingDefSource(MeshDefinition& mesh, 
                        std::vector<MaterialDefinition>& materials)
         : m_mesh(mesh)
         , m_materials(materials)
      {}

      void parseMesh(MeshDefinition& mesh, 
                     AnimationDefinition& animation, 
                     std::vector<MaterialDefinition>& materials,
                     const std::string& name)
      {
         mesh = m_mesh;
         materials.insert(materials.end(), m_materials.begin(), m_materials.end());
      }
   };

   class StaticGeometryOutputAdapter : public StaticGeometry
   {
   private:
      GraphicalEntitiesFactory& m_factory;
      StaticGeometrySetter& m_addObj;

   public:
      StaticGeometryOutputAdapter(GraphicalEntitiesFactory& factory,
                                  StaticGeometrySetter& addObj)
         : m_factory(factory)
         , m_addObj(addObj)
      {
      }

      void addMesh(const std::vector<LitVertex>& vertices,
                   const std::vector<Face<USHORT> >& faces,
                   Material& material)
      {
         m_addObj(m_factory.createStaticGeometry(material, vertices, faces));
      }
   };

   D3DXMatrixMultiply(&(mesh.localMtx), &(mesh.localMtx), &situation);
   ExistingDefSource dataSource(mesh, materials);

   StaticGeometryOutputAdapter output(m_entitiesFactory, m_addStaticObj);

   m_staticGeometryLoader->load(m_materialsStorage, dataSource, output);
}

///////////////////////////////////////////////////////////////////////////////

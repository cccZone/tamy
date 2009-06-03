#include "core-ResourceManagement\ResourceManager.h"
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\SkinnedGraphicalEntity.h"
#include "core-Renderer\CompositeGraphicalEntity.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "core-ResourceManagement\MaterialsParser.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core\Node.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\SkyBox.h"
#include "core-ResourceManagement\Managable.h"
#include "core-ResourceManagement\FileGraphicalEntityLoader.h"
#include "core-Sound\SoundRenderer.h"
#include "core-ResourceManagement\XMLFont.h"
#include <cassert>
#include <deque>


///////////////////////////////////////////////////////////////////////////////

ResourceManager::ResourceManager(const std::string& texturesDirPath)
      : m_materialsParser(NULL),
      m_texturesDirPath(texturesDirPath),
      m_soundRenderer(NULL)
{
   m_materialsParser = new MaterialsParser(*this);
}

///////////////////////////////////////////////////////////////////////////////

ResourceManager::~ResourceManager()
{
   for (std::list<Managable*>::iterator it = m_allObjects.begin(); it != m_allObjects.end(); ++it)
   {
      delete *it;
   }
   m_allObjects.clear();

   m_materials.clear();
   m_materialsByName.clear();
   m_graphicalEntities.clear();
   m_lightReflectingProperties.clear();
   m_textures.clear();
   m_fonts.clear();

   for (std::list<FileGraphicalEntityLoader*>::iterator it = m_graphicalEntitiesLoaders.begin();
      it != m_graphicalEntitiesLoaders.end(); ++it)
   {
      delete *it;
   }
   m_graphicalEntitiesLoaders.clear();

   delete m_soundRenderer;
   m_soundRenderer = NULL;

   delete m_materialsParser;
   m_materialsParser = NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool ResourceManager::isGraphicalEntityRegistered(const std::string& name) const
{
   return (m_graphicalEntities.find(name) != m_graphicalEntities.end());
}

///////////////////////////////////////////////////////////////////////////////

void ResourceManager::registerGraphicalEntity(const std::string& name,
                                              AbstractGraphicalEntity* entity)
{
   if (m_graphicalEntities.find(name) != m_graphicalEntities.end())
   {
      throw std::logic_error(
         std::string("GraphicalEntity ") + name + std::string(" is already registered"));
   }

   m_allObjects.push_back(new TManagable<AbstractGraphicalEntity>(entity));
   m_graphicalEntities.insert(std::make_pair(name, entity));
}

///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity& ResourceManager::loadGraphicalEntity(const std::string& name, 
                                                              GraphicalEntityLoader& loader)
{
   if (m_graphicalEntities.find(name) != m_graphicalEntities.end())
   {
      throw std::logic_error(
         std::string("GraphicalEntity ") + name + std::string(" is already registered"));
   }

   return loader.load(*this, name);
}

///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity& ResourceManager::getGraphicalEntity(const std::string& name)
{
   std::map<std::string, AbstractGraphicalEntity*>::iterator it = m_graphicalEntities.find(name);
   if (it == m_graphicalEntities.end())
   {
      throw std::out_of_range(
         std::string("GraphicalEntity ") + name + std::string(" is not registered"));
   }
   return *(it->second);
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntityLoader& ResourceManager::getLoaderForFile(const std::string& fileName)
{
   for (std::list<FileGraphicalEntityLoader*>::iterator it = m_graphicalEntitiesLoaders.begin();
      it != m_graphicalEntitiesLoaders.end(); ++it)
   {
      FileGraphicalEntityLoader& loader = **it;
      if (loader.canHandleFile(fileName))
      {
         return loader;
      }
   }

   throw std::out_of_range(
      std::string("There's no loader capable of loading the mesh file ") + fileName);
}

///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity* ResourceManager::createGraphicalEntityFromTemplate(MeshDefinition& mesh)
{
   // register the meshes
   CompositeGraphicalEntity* entityRootNode = new CompositeGraphicalEntity(mesh.name, mesh.localMtx);
   std::deque<std::pair<CompositeGraphicalEntity*, MeshDefinition*> > meshesQueue;
   meshesQueue.push_back(std::make_pair(entityRootNode, &mesh));

   while (meshesQueue.size() > 0)
   {
      CompositeGraphicalEntity* parentEntity = meshesQueue.front().first;
      MeshDefinition& currentMesh = *(meshesQueue.front().second);
      meshesQueue.pop_front();
      
      // create a graphical entity based on the geometry definition
      std::vector<Material*> realMaterials;
      getMaterials(currentMesh.materials, realMaterials);

      if (currentMesh.faces.size() > 0)
      {
         AbstractGraphicalEntity* currentEntity = NULL;

         if (currentMesh.isSkin)
         {
            currentEntity = createSkinedGraphicalEntity(
                                          currentMesh.name + "_entity",
                                          currentMesh,
                                          realMaterials);
         }
         else
         {
            currentEntity = createGraphicalEntity(currentMesh.name + "_entity",
                                                  currentMesh,
                                                  realMaterials);
         }
         parentEntity->addChild(currentEntity);
      }

      // add the mesh's children to the queue
      for (std::list<MeshDefinition*>::iterator childrenIt = currentMesh.children.begin();
           childrenIt != currentMesh.children.end(); ++childrenIt)
      {
         CompositeGraphicalEntity* childNode = new CompositeGraphicalEntity((*childrenIt)->name, 
                                                                            (*childrenIt)->localMtx);
         parentEntity->addChild(childNode);
         meshesQueue.push_back(std::make_pair(childNode, *childrenIt));
      }
   }


   return entityRootNode;
}

///////////////////////////////////////////////////////////////////////////////

void ResourceManager::getMaterials(const std::vector<MaterialDefinition>& inMaterialDefinitions,
                                   std::vector<Material*>& outRealMaterials)
{
   // split the meshes by the materials
   outRealMaterials.resize(inMaterialDefinitions.size());
   for (unsigned int matIdx = 0; matIdx < inMaterialDefinitions.size(); ++matIdx)
   {
      const MaterialDefinition& mat = inMaterialDefinitions.at(matIdx);

      Material* realMat = NULL;
      if (doesMaterialExist(mat.matName) == true)
      {
         realMat = &findMaterial(mat.matName);
      }
      else
      {
         Texture* texture = NULL;
         if (isTextureRegistered(mat.texName))
         {
            texture = &getTexture(mat.texName);
         }
         else
         {
            texture = &loadTexture(mat.texName);
         }

         LightReflectingProperties* lrp  = createLightReflectingProperties();
         lrp->setAmbientColor(mat.ambient);
         lrp->setDiffuseColor(mat.diffuse);
         lrp->setSpecularColor(mat.specular);
         lrp->setEmissiveColor(mat.emissive);
         lrp->setPower(mat.power);
         lrp = &addLightReflectingProperties(lrp);

         realMat = &createMaterial(mat.matName, *lrp);
         MaterialStage* defaultStage = createMaterialStage(*texture,
                                                           MOP_MULTIPLY, SC_LRP, SC_TEXTURE,
                                                           MOP_DISABLE, SC_NONE, SC_NONE);
         realMat->addStage(defaultStage);
      }

      outRealMaterials[matIdx] = realMat;
      
   }
}

///////////////////////////////////////////////////////////////////////////////

Camera* ResourceManager::createCamera(const std::string& name)
{
   Camera* node = new Camera(name);
   return node;
}

///////////////////////////////////////////////////////////////////////////////

Light* ResourceManager::createLight(const std::string& name)
{
   Light* node = createLightImpl(name);
   return node;
}

///////////////////////////////////////////////////////////////////////////////

void ResourceManager::loadMaterialDefinition(const std::string& fileName)
{
   m_materialsParser->load(m_texturesDirPath + std::string("\\") + fileName);
}

///////////////////////////////////////////////////////////////////////////////

bool ResourceManager::doesMaterialExist(const std::string& name) const
{
   std::map<std::string, Material*>::const_iterator it = m_materialsByName.find(name);
   return (it != m_materialsByName.end());
}

///////////////////////////////////////////////////////////////////////////////

Material& ResourceManager::findMaterial(const std::string& name)
{
   std::map<std::string, Material*>::iterator it = m_materialsByName.find(name);
   if (it == m_materialsByName.end())
   {
      throw std::out_of_range(std::string("Invalid material name"));
   }

   return *(it->second);
}

///////////////////////////////////////////////////////////////////////////////

Material& ResourceManager::getMaterial(unsigned int id)
{
   if (id >= m_materials.size())
   {
      throw std::out_of_range(std::string("Invalid material id"));
   }

   return *(m_materials.at(id));
}

///////////////////////////////////////////////////////////////////////////////

Material& ResourceManager::createMaterial(const std::string& materialName, 
                                          LightReflectingProperties& lrp)
{
   std::map<std::string, Material*>::iterator it = m_materialsByName.find(materialName);
   if (it != m_materialsByName.end())
   {
      throw std::runtime_error(std::string("Material") + materialName + std::string("already exists"));
   }

   Material* newMaterial = createMaterialImpl(lrp, m_materials.size());
   m_materials.push_back(newMaterial);
   m_materialsByName.insert(std::make_pair(materialName, newMaterial));
   m_allObjects.push_back(new TManagable<Material>(newMaterial));

   return *newMaterial;
}

///////////////////////////////////////////////////////////////////////////////

bool ResourceManager::areLightReflectingPropertiesRegistered(const LightReflectingProperties& lrp) const
{
   for (std::list<LightReflectingProperties*>::const_iterator it = m_lightReflectingProperties.begin();
      it != m_lightReflectingProperties.end(); ++it)
   {
      if (**it == lrp) {return true;}
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

LightReflectingProperties& ResourceManager::getLightReflectingProperties(const LightReflectingProperties& lrp) const
{
   for (std::list<LightReflectingProperties*>::const_iterator it = m_lightReflectingProperties.begin();
      it != m_lightReflectingProperties.end(); ++it)
   {
      if (**it == lrp) 
      {
         return **it;
      }
   }

   throw std::out_of_range(std::string("LightReflectingProperties like that not defined"));
}

///////////////////////////////////////////////////////////////////////////////

LightReflectingProperties& ResourceManager::addLightReflectingProperties(LightReflectingProperties* lrp)
{
   if (areLightReflectingPropertiesRegistered(*lrp))
   {
      LightReflectingProperties& existingLRP = getLightReflectingProperties(*lrp);
      delete lrp;
      return existingLRP;
   }
   else
   {
      m_lightReflectingProperties.push_back(lrp);
      m_allObjects.push_back(new TManagable<LightReflectingProperties>(lrp));
      return *lrp;
   }
}

///////////////////////////////////////////////////////////////////////////////

bool ResourceManager::isTextureRegistered(const std::string& name) const
{
   std::map<std::string, Texture*>::const_iterator it = m_textures.find(name);
   return (it != m_textures.end());
}

///////////////////////////////////////////////////////////////////////////////

Texture& ResourceManager::loadTexture(const std::string& name)
{
   if (isTextureRegistered(name) == true)
   {
      throw std::logic_error(std::string("Texture <") + name + std::string("> is already registered"));
   }

   if (name.length() == 0)
   {
      return getEmptyTexture();
   }
   else
   {
      Texture* tex = loadTexture(m_texturesDirPath, name);
      m_textures.insert(std::make_pair(name, tex));
      m_allObjects.push_back(new TManagable<Texture>(tex));

      return *tex;
   }
}

///////////////////////////////////////////////////////////////////////////////

Texture& ResourceManager::getTexture(const std::string& name) const
{
   std::map<std::string, Texture*>::const_iterator it = m_textures.find(name);
   if (it == m_textures.end())
   {
      throw std::out_of_range(std::string("Texture <") + name + std::string("> is not registered"));
   }

   return *(it->second);
}

///////////////////////////////////////////////////////////////////////////////

Texture& ResourceManager::getEmptyTexture()
{
   if (isTextureRegistered(""))
   {
      return getTexture("");
   }
   else
   {
      Texture* tex = createEmptyTexture();
      m_textures.insert(std::make_pair("", tex));
      m_allObjects.push_back(new TManagable<Texture>(tex));
      return *tex;
   }
}

///////////////////////////////////////////////////////////////////////////////

SkyBox* ResourceManager::createSkyBox()
{
   SkyBox* skyBox = createSkyBoxImpl();
   return skyBox;
}

///////////////////////////////////////////////////////////////////////////////

MaterialStage* ResourceManager::createMaterialStage(Texture& texture,
                                      MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                                      MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2)
{
   return new MaterialStage(texture,
         new MaterialOperation(getColorOperationImpl(), colorOp, colorArg1, colorArg2),
         new MaterialOperation(getAlphaOperationImpl(), alphaOp, alphaArg1, alphaArg2));
}

///////////////////////////////////////////////////////////////////////////////

void ResourceManager::registerFileGraphicalEntityLoader(FileGraphicalEntityLoader* loader)
{
   if (loader == NULL)
   {
      throw std::invalid_argument(
         std::string("NULL pointer instead a FileGraphicalEntityLoader instance"));
   }

   m_graphicalEntitiesLoaders.push_back(loader);
}

///////////////////////////////////////////////////////////////////////////////

SoundRenderer& ResourceManager::getSoundRenderer()
{
   if (m_soundRenderer == NULL)
   {
      m_soundRenderer = new SoundRenderer(getSoundDeviceInstance());
   }

   return *m_soundRenderer;
}

///////////////////////////////////////////////////////////////////////////////

bool ResourceManager::isFontRegistered(const std::string& name) const
{
   std::map<std::string, Font*>::const_iterator it = m_fonts.find(name);
   return (it != m_fonts.end());
}

///////////////////////////////////////////////////////////////////////////////

Font& ResourceManager::getFont(const std::string& name)
{
   std::map<std::string, Font*>::const_iterator it = m_fonts.find(name);
   if (it == m_fonts.end())
   {
      throw std::out_of_range(std::string("Font <") + name + std::string("> is not registered"));
   }

   return *(it->second);
}

///////////////////////////////////////////////////////////////////////////////

void ResourceManager::loadFont(const std::string& name)
{
   std::map<std::string, Font*>::const_iterator it = m_fonts.find(name);
   if (it != m_fonts.end())
   {
      throw std::out_of_range(std::string("Font <") + name + std::string("> already registered"));
   }

   XMLFont* font = new XMLFont((m_texturesDirPath + std::string("\\") + name).c_str(), *this);
   m_allObjects.push_back(new TManagable<Font>(font));
   m_fonts.insert(std::make_pair(name, font));
}

///////////////////////////////////////////////////////////////////////////////

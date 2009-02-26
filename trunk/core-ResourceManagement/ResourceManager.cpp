#include "ResourceManager.h"
#include "GraphicalNode.h"
#include "GraphicalEntity.h"
#include "GraphicalEntityLoader.h"
#include "Camera.h"
#include "Light.h"
#include "Node.h"
#include "Renderer.h"
#include "Material.h"
#include "LightReflectingProperties.h"
#include "Texture.h"
#include "SkyBox.h"
#include "Managable.h"
#include "FileGraphicalEntityLoader.h"
#include "SpacerGraphicalEntity.h"
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

ResourceManager::ResourceManager(const std::string& texturesDirPath)
      : m_texturesDirPath(texturesDirPath)
{
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
   m_graphicalEntities.clear();
   m_lightReflectingProperties.clear();
   m_textures.clear();

   for (std::list<FileGraphicalEntityLoader*>::iterator it = m_graphicalEntitiesLoaders.begin();
      it != m_graphicalEntitiesLoaders.end(); ++it)
   {
      delete *it;
   }
   m_graphicalEntitiesLoaders.clear();
}

///////////////////////////////////////////////////////////////////////////////

bool ResourceManager::isGraphicalEntityRegistered(const std::string& name) const
{
   return (m_graphicalEntities.find(name) != m_graphicalEntities.end());
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntity& ResourceManager::createGraphicalEntity(const std::string& name,
                                                        const MeshDefinition& subMesh)
{
   if (m_graphicalEntities.find(name) != m_graphicalEntities.end())
   {
      throw std::logic_error(
         std::string("GraphicalEntity ") + name + std::string(" is already registered"));
   }

   GraphicalEntity* entity;

   if ((subMesh.faces.size() == 0) || (subMesh.vertices.size() == 0))
   {
      entity = new SpacerGraphicalEntity(name, subMesh.localMtx);
   }
   else
   {
      // split the meshes by the materials
      std::vector<Material*> realMaterials(subMesh.materials.size());
      for (unsigned int matIdx = 0; matIdx < subMesh.materials.size(); ++matIdx)
      {
         const MaterialDefinition& mat = subMesh.materials.at(matIdx);
         unsigned int id = addMaterial(mat.texName, mat.ambient, 
                                       mat.diffuse, mat.specular, 
                                       mat.emissive, mat.power);
         realMaterials[matIdx] = &getMaterial(id);
      }

      entity = createGraphicalEntityImpl(name, subMesh, realMaterials);
   }
   m_allObjects.push_back(new TManagable<GraphicalEntity>(entity));
   m_graphicalEntities.insert(std::make_pair(name, entity));

   return *entity;
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntity& ResourceManager::loadGraphicalEntity(const std::string& name, GraphicalEntityLoader& loader)
{
   if (m_graphicalEntities.find(name) != m_graphicalEntities.end())
   {
      throw std::logic_error(
         std::string("GraphicalEntity ") + name + std::string(" is already registered"));
   }

   return loader.load(*this, name);
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntity& ResourceManager::getGraphicalEntity(const std::string& name)
{
   std::map<std::string, GraphicalEntity*>::iterator it = m_graphicalEntities.find(name);
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

Material& ResourceManager::getMaterial(unsigned int id)
{
   // there's always a default material - if there's none - make one
   if (m_materials.size() == 0)
   {
      addMaterial("", Color(), Color(1, 1, 1, 1), Color(1, 1, 1, 1), Color(), 1);
   }

   if (id >= m_materials.size())
   {
      throw std::out_of_range(std::string("Invalid material id"));
   }

   return *(m_materials.at(id));
}

///////////////////////////////////////////////////////////////////////////////

unsigned int ResourceManager::addMaterial(const std::string& textureName, 
                                          const Color& ambient,
                                          const Color& diffuse,
                                          const Color& specular,
                                          const Color& emissive,
                                          float power)
{
   Material* materialCandidate = new Material(getEmptyTexture(), m_materials.size());

   // get light reflectance properties
   LightReflectingProperties* lrpCandidate = createLightReflectingProperties();
   lrpCandidate->setAmbientColor(ambient);
   lrpCandidate->setDiffuseColor(diffuse);
   lrpCandidate->setSpecularColor(specular);
   lrpCandidate->setEmissiveColor(emissive);
   lrpCandidate->setPower(power);
   LightReflectingProperties* lrp = NULL;
   if (areLightReflectingPropertiesRegistered(*lrpCandidate))
   {
      lrp = &getLightReflectingProperties(*lrpCandidate);
      delete lrpCandidate;
   }
   else
   {
      lrp = &addLightReflectingProperties(lrpCandidate);
   }
   assert(lrp != NULL);
   materialCandidate->setLightReflectingProperties(*lrp);

   // get a texture
   Texture* texture = NULL;
   if (isTextureRegistered(textureName))
   {
      texture = &getTexture(textureName);
   }
   else
   {
      texture = &loadTexture(textureName);
   }
   assert(texture != NULL);
   materialCandidate->setTexture(*texture);
   
   // check if there's a material with matching properties out there
   unsigned int id = materialCandidate->getIndex();
   for (std::vector<Material*>::iterator it = m_materials.begin(); it != m_materials.end(); ++it)
   {
      Material& existingMat = **it;
      if (existingMat == *materialCandidate)
      {
         delete materialCandidate;
         return existingMat.getIndex();
      }
   }

   m_materials.push_back(materialCandidate);
   m_allObjects.push_back(new TManagable<Material>(materialCandidate));
   return id;
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
      throw std::logic_error(std::string("These light reflecting properties have already been registered"));
   }

   m_lightReflectingProperties.push_back(lrp);
   m_allObjects.push_back(new TManagable<LightReflectingProperties>(lrp));
   return *lrp;
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

   Texture* tex = loadTexture(m_texturesDirPath, name);
   m_textures.insert(std::make_pair(name, tex));
   m_allObjects.push_back(new TManagable<Texture>(tex));

   return *tex;
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

SkyBox& ResourceManager::createSkyBox()
{
   SkyBox* skyBox = createSkyBoxImpl();
   m_allObjects.push_back(new TManagable<SkyBox>(skyBox));

   return *skyBox;
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

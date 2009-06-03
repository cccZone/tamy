#pragma once

#include <list>
#include <vector>
#include <map>
#include <string>
#include "core-ResourceManagement\Face.h"
#include "core-ResourceManagement\LitVertex.h"
#include "core-Renderer\MaterialOperation.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialsParser;
class Node;
class Camera;
class Light;
class Renderer;
class AbstractGraphicalEntity;
class LightReflectingProperties;
class Texture;
class Material;
class MaterialStage;
class MaterialOperationImplementation;
struct Color;
class Managable;
class SkyBox;
class GraphicalEntityLoader;
class FileGraphicalEntityLoader;
struct MeshDefinition;
struct AnimationDefinition;
class Skeleton;
class GraphicalEntity;
class SkinnedGraphicalEntity;
struct SkinBoneDefinition;
struct MaterialDefinition;
class SoundDevice;
class SoundRenderer;
class SoundListener;
class Sound3D;
class Sound;
class Font;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class is responsible for managing the instances of objects
 * we use to populate our scene - creating and deleting them, 
 * instanciating them if applicable etc.
 */
class ResourceManager
{
private:
   MaterialsParser* m_materialsParser;

   std::list<Managable*> m_allObjects;
   std::map<std::string, AbstractGraphicalEntity*> m_graphicalEntities;
   std::vector<Material*> m_materials;
   std::map<std::string, Material*> m_materialsByName;
   std::list<LightReflectingProperties*> m_lightReflectingProperties;
   std::map<std::string, Texture*> m_textures;
   std::map<std::string, Font*> m_fonts;
   std::string m_texturesDirPath;

   std::list<FileGraphicalEntityLoader*> m_graphicalEntitiesLoaders;

   SoundRenderer* m_soundRenderer;

public:
   ResourceManager(const std::string& texturesDirPath);

   virtual ~ResourceManager();

   /**
    * This method returns an instance of the render consistent
    * with other entities implementations.
    */
   virtual Renderer& getRendererInstance() = 0;

   // --------------------------- Graphical Entities --------------------------

   /**
    * Checks whether a graphical object with a specified
    * name already exists
    */
   bool isGraphicalEntityRegistered(const std::string& name) const;

   /**
    * This method can be used to register an entity with the system
    * for further management
    *
    * @throws std::logic_error if an entity with the specified name already exists
    */
    void registerGraphicalEntity(const std::string& name,
                                 AbstractGraphicalEntity* entity);

   /**
    * Loads a mesh using a graphical entity loader. This method DOES
    * register the entity with the manager - you don't have to call 'registerGraphicalEntity'
    * afterwards
    *
    * @throws std::logic_error if an entity with the specified name already exists
    * @param name - name of the file from which the mesh should be loaded.
    *               It's also the name under which the mesh will be registered.
    */
   AbstractGraphicalEntity& loadGraphicalEntity(const std::string& name, GraphicalEntityLoader& loader);

   /**
    * Rturns a reference to a graphical entity providing one exists
    *
    * @throws std::out_of_range exception if one hasn't been registered yet
    */
   AbstractGraphicalEntity& getGraphicalEntity(const std::string& name);

   /**
    * Returns a loader capable of dealing with a specified file type,
    * providing one exists.
    *
    * @throws std::out_of_range if there's no loader that's capable
    *                           of dealing with the specified file type
    */
   GraphicalEntityLoader& getLoaderForFile(const std::string& fileName);

   /**
    * This method provides a mechanism for creating a complete greaphical entity
    * based on the data from the mesh definition.
    *
    * The entity does not get registered as a resource automatically.
    */
   AbstractGraphicalEntity* createGraphicalEntityFromTemplate(MeshDefinition& mesh);

   // -------------------------------- Cameras --------------------------------

   Camera* createCamera(const std::string& name);

   // --------------------------------- Lights ---------------------------------

   Light* createLight(const std::string& name);

   // ------------------------------- Materials --------------------------------

   /**
    * The method loads the materials definition file from the materials directory
    * (which at this point is the same as the textures directory ;)
    */
   void loadMaterialDefinition(const std::string& fileName);

   /**
    * The method checks if particular set of light reflecting properties is registered
    */
   bool areLightReflectingPropertiesRegistered(const LightReflectingProperties& lrp) const;

   /**
    * Retrieves the light reflecting properties matching those 
    * of the sample passed as an argument
    */
   LightReflectingProperties& getLightReflectingProperties(const LightReflectingProperties& lrp) const;

   /** 
    * Create a new instance of light reflecting properties
    */
   virtual LightReflectingProperties* createLightReflectingProperties() = 0;

   /**
    * Registers new set of light reflecting properties
    */
   LightReflectingProperties& addLightReflectingProperties(LightReflectingProperties* lrp);

   /**
    * Checks whether a texture with a certain name has been already created
    */
   bool isTextureRegistered(const std::string& name) const;

   /**
    * This method loads a texture with a given name from a file.
    * The name of the file should match that of the texture name specified.
    *
    * @throws std::logic_error is a texture with the specified name already exists
    */
   Texture& loadTexture(const std::string& name);

   /**
    * This method retrieves a pointer to a texture with a specified name.
    *
    * @throws std::out_of_range exception if one hasn't been registered yet
    */
   Texture& getTexture(const std::string& name) const;

   /**
    * Returns a material with the given id, if one exists.
    * @throws std::out_of_range exception if one hasn't been defined
    */
   Material& getMaterial(unsigned int id);

   /**
    * Checks if a material with the given name exists.
    */
   bool doesMaterialExist(const std::string& name) const;

   /**
    * Returns a material with the given name, if one exists.
    * @throws std::out_of_range exception if one hasn't been defined
    */
   Material& findMaterial(const std::string& name);

   /**
    * This method adds a new material and returns an ID assigned to it.
    */
   Material& createMaterial(const std::string& materialName, LightReflectingProperties& lrp);

   /**
    * This method creates a new material stage
    */
   MaterialStage* createMaterialStage(Texture& texture,
                                      MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                                      MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2);

   // ------------------------------ Fonts ------------------------------

   /**
    * The method checks if a font with the given name is loaded
    */
   bool isFontRegistered(const std::string& name) const;

   /**
    * The method retrieves an instance of the font with the specified name
    */
   Font& getFont(const std::string& name);

   /**
    * The method loads a font from the specified file.
    *
    * The file must be located in the path specified by the textureDirPath param
    * passed into the constructor when the instance of the ResourceManager
    * was created
    */
   void loadFont(const std::string& name);

   // ------------------------------ Decorations ------------------------------

   /**
    * This method creates a sky box instance
    */
   SkyBox* createSkyBox();

   // ------------------------------ Sound system ------------------------------

   /**
    * The method returns the sole instance of a sound device created for the application
    */
   virtual SoundDevice& getSoundDeviceInstance() = 0;

   /**
    * The method returns the sole instance of the sound renderer
    */
   SoundRenderer& getSoundRenderer();

   /**
    * The method creates an implementation specific instance of a sound listener
    */
   virtual SoundListener* createSoundListener() = 0;

   /**
    * The method creates an implementation specific instance of a sound that can
    * be placed in the 3D scene
    */
   virtual Sound3D* createSound3D(const std::string& name, 
                                  Sound& sound, 
                                  float hearingRadius) = 0;

protected:

      /**
    * This method should return a valid implementation of the GraphicalEntity class
    */
   virtual GraphicalEntity* createGraphicalEntity(const std::string& name,
                          const MeshDefinition& subMesh,
                          const std::vector<Material*>& registeredMaterials) = 0;

   /**
    * This method should return a valid implementation of the GraphicalEntity class
    */
   virtual SkinnedGraphicalEntity* createSkinedGraphicalEntity(const std::string& name,
                          const MeshDefinition& subMesh,
                          const std::vector<Material*>& registeredMaterials) = 0;

   /**
    * This method should return a valid implementation of the Light class
    */
   virtual Light* createLightImpl(const std::string& name) = 0;

   virtual Texture* createEmptyTexture() = 0;

   virtual Texture* loadTexture(const std::string& textureDirPath, const std::string& name) = 0;

   virtual SkyBox* createSkyBoxImpl() = 0;

   virtual Material* createMaterialImpl(LightReflectingProperties& lrp, unsigned int index) = 0;

   virtual MaterialOperationImplementation& getColorOperationImpl() = 0;

   virtual MaterialOperationImplementation& getAlphaOperationImpl() = 0;

   /**
    * This method allows the implementations to register
    * the mesh file handlers specific for a particular implementation type
    */
   void registerFileGraphicalEntityLoader(FileGraphicalEntityLoader* loader);

private:
   Texture& getEmptyTexture();

   void getMaterials(const std::vector<MaterialDefinition>& inMaterialDefinitions,
                     std::vector<Material*>& outRealMaterials);
};

///////////////////////////////////////////////////////////////////////////////

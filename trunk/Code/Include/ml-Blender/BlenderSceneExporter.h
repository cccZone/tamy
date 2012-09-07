/// @file   ml-Blender/BlenderSceneExporter.h
/// @brief  this class exports blender scenes to Tamy
#pragma once

#include <vector>
#include "core/types.h"
#include "ml-Blender/TamyMaterial.h"


///////////////////////////////////////////////////////////////////////////////

struct TamyMesh;
struct TamyScene;
struct TamyGeometry;
struct TamyTexture;
struct TamyLight;
class SpatialEntity;
class TriangleMesh;
class Geometry;
class Texture;
class Material;

///////////////////////////////////////////////////////////////////////////////

class BlenderSceneExporter
{
private:
   /**
    * A structure dedicated to storing the material settings. We inherit from TamyMaterial 
    * structure on purpose here, in order to make sure all the data is there and will be copied
    * in the only constructor, which also happens to be a copy constructor, for two reasons:
    * - if anything changes in the base structure, the compiler will remind us to update the copying code as well.
    * - because the base structure is used only during the export, and once the call to the export method
    *   is over, the structures go out of scope. But since we want to use that data for lookup purposes later on, we need to copy it.
    *
    * So since we don't want to end up with a mess of dependencies, we separate the two structures 
    * ( the one used for export from the one used for lookup ). That's because the latter has some
    * additional operations that release the memory allocated for internal arrays etc, while the former
    * is just used to give a meaning to the exported memory chunk.
    */
   struct MaterialDefinition : public TamyMaterial
   {
      /**
       * Copy constructor.
       *
       * @param rhs
       */
      MaterialDefinition( const TamyMaterial& rhs );
      ~MaterialDefinition();
   };

   struct ExportedEntity
   {
      SpatialEntity*                         m_entity;
      int                                    m_parentIndex;
   };

private:
   static BlenderSceneExporter               s_theIntance;

   Material*                                 m_material;
   std::string                               m_texturesExportDir;
   std::string                               m_meshesExportDir;
   std::string                               m_scenesExportDir;

   std::vector< Texture* >                   m_textures;
   std::vector< MaterialDefinition* >        m_materialDefinitions;
   std::vector< ExportedEntity >             m_exportedEntities;
   uint                                      m_nextEntityIdx;

public:
   /**
    * Returns the singleton instance of the class.
    */
   static BlenderSceneExporter& getInstance() { return s_theIntance; }

   /**
    * Initializes the exporter.
    *
    * @param filesystemRoot
    * @param exportDir
    * @param entitiesCount
    */
   void initialize( const char* filesystemRoot, const char* exportDir, int entitiesCount );

   /**
    * Purges all temporary buffers, preparing the singleton instance for the next use.
    */
   void reset();

   /**
    * Creates a texture resource.
    *
    * @param exportedTexture
    */
   void createTexture( const TamyTexture& exportedTexture );

   /**
    * Stores the materials for further lookups.
    *
    * @param arrMaterials
    * @param materialsCount
    */
   void storeMaterials( TamyMaterial* arrMaterials, int materialsCount );

   /**
    * Adds an entity to the collection of exported entities under the correct index.
    *
    * @param entity
    * @param parentIdx
    */
   void addEntity( SpatialEntity* entity, int parentIdx );

   /**
    * Creates a mesh resource.
    *
    * @param exportedMesh
    */
   TriangleMesh* createMesh( const TamyMesh& exportedMesh );

   /**
    * Creates a geometry entity
    *
    * @param exportedGeometryEntity
    */
   SpatialEntity* createGeometryEntity( const TamyGeometry& exportedGeometryEntity );

   /**
    * Creates a light entity.
    *
    * @param exportedLightEntity
    */
   SpatialEntity* createLightEntity( const TamyLight& exportedLightEntity );

   /**
    * Creates a scene resource.
    *
    * @param sceneName
    */
   void assembleScene( const char* sceneName );

private:
   /**
    * Constructor.
    */
   BlenderSceneExporter();

   /**
    * Creates and attaches a material entity to the specified geometry entity.
    * The material attached is taken from the specified definition that ought to be 
    * loaded before a call to this method was made.
    *
    * @param geometryEntity
    * @param materialIdx
    */
   void attachMaterialToGeometry( Geometry* geometryEntity, uint materialIdx ) const;

   /**
    * This helper method instantiates a single StaticGeometry entity according to the specified description.
    *
    * @param exportedGeometryEntity
    * @param meshIdx
    * @param useOriginalName        ( tells if the entity should be named exactly after the exportedGeometryEntity, or should a mesh index be appended to that name )
    */
   Geometry* instantiateStaticGeometry( const TamyGeometry& exportedGeometryEntity, int meshIdx, bool useOriginalName );

   /**
    * Instantiates a single directional light entity.
    *
    * @param exportedLightEntity
    */
   SpatialEntity* instantiateDirectionalLight( const TamyLight& exportedLightEntity );
   
   /**
    * Instantiates a single point light entity.
    *
    * @param exportedLightEntity
    */
   SpatialEntity* instantiatePointLight( const TamyLight& exportedLightEntity );
};

///////////////////////////////////////////////////////////////////////////////

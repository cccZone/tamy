/// @file   ml-Blender/BlenderSceneExporter.h
/// @brief  this class exports blender scenes to Tamy
#pragma once

#include <vector>
#include "core/types.h"
#include "ml-Blender/TamyMaterial.h"
#include "ml-Blender/BaseExportStructs.h"
#include "core/Matrix.h"
#include "core/FilePath.h"
#include "core/Array.h"


///////////////////////////////////////////////////////////////////////////////

struct TamyMesh;
struct TamyScene;
struct TamyGeometry;
struct TamyTexture;
struct TamyLight;
struct TamyWorld;
class SpatialEntity;
class TriangleMesh;
class Geometry;
class Texture;
class Material;
class Entity;

///////////////////////////////////////////////////////////////////////////////

class BlenderSceneExporter
{
private:
   struct MaterialDefinition
   {
      std::string    m_materialName;
      FilePath       m_path;
   };

   ALIGN_16 struct ExportedEntity
   {
      SpatialEntity*                         m_entity;
      Matrix                                 m_globalMtx;
      int                                    m_parentIndex;
   };

private:
   static BlenderSceneExporter               s_theIntance;

   Material*                                 m_materialRenderer;
   std::string                               m_texturesExportDir;
   std::string                               m_meshesExportDir;
   std::string                               m_scenesExportDir;
   std::string                               m_materialsExportDir;

   std::vector< Texture* >                   m_textures;
   std::vector< MaterialDefinition >         m_materialDefinitions;
   Array< ExportedEntity >                   m_exportedEntities;
   uint                                      m_nextEntityIdx;
   std::vector< Entity* >                    m_worldEntities;

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
    * @param transform
    * @param parentIdx
    */
   void addEntity( SpatialEntity* entity, const Matrix& transform, int parentIdx );

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
    * Sets up global rendering data.
    *
    * @param worldSettings
    */
   void setupWorld( const TamyWorld& worldSettings );

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

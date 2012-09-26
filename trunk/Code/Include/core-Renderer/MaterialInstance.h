/// @file   core-Renderer/MaterialInstance.h
/// @brief  a customized material instance, that supplies all data a material resource requires
#pragma once

#include "core/Resource.h"
#include "core/types.h"
#include "core-Renderer/SurfaceProperties.h"
#include "core/Observer.h"
#include "core/Subject.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class Material;
class RuntimeDataBuffer;
class MaterialNode;
enum GraphBuilderOperation;
enum GraphBuilderNodeOperation;
class Texture;
class MaterialEntity;

///////////////////////////////////////////////////////////////////////////////

enum MaterialTextures
{
   MT_DIFFUSE_1,
   MT_DIFFUSE_2,
   MT_NORMALS,
   MT_SPECULAR,

   MT_MAX
};

///////////////////////////////////////////////////////////////////////////////

enum MaterialInstanceOperation
{
   MIO_MAT_CHANGED
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A customized material instance, that supplies all data a material resource requires.
 *
 * A material instance is a render state, meaning that it can and should be used
 * to efficiently batch materials during rendering.
 *
 */
class MaterialInstance : public Resource,
                        public Observer< Material, GraphBuilderOperation >,
                        public Observer< MaterialNode, GraphBuilderNodeOperation >,
                        public Subject< MaterialInstance, MaterialInstanceOperation >
{
   DECLARE_RESOURCE()


private:
   // static data
   SurfaceProperties                         m_surfaceProperties;
   Material*                                 m_materialRenderer;
   std::vector< Texture* >                   m_texture;

   // runtime data

   // index of this material assigned to it by the materials DB it's registered with
   uint                                      m_index;

   // material-related  runtime data
   RuntimeDataBuffer*                        m_dataBuf;
   std::vector< MaterialNode* >              m_nodesQueue;

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   MaterialInstance( const FilePath& resourceName = FilePath() );
   ~MaterialInstance();

   /**
    * Returns a unique instance this material is identified with.
    */
   inline uint getIndex() const { return m_index; }

   /**
    * Sets a new material resource this instance should use to render stuff.
    *
    * @param material
    */
   void setMaterialRenderer( Material* materialRenderer );

   /**
    * Returns a pointer to a material this instance is using.
    */
   inline Material* getMaterialRenderer() const { return m_materialRenderer; }

   /**
    * Returns an instance of surface properties of this material instance ( non-const version )
    */
   inline SurfaceProperties& accessSurfaceProperties() { notify( MIO_MAT_CHANGED ); return m_surfaceProperties; }

   /**
    * Returns an instance of surface properties of this material instance.
    */
   inline const SurfaceProperties& getSurfaceProperties() const { return m_surfaceProperties; }

   /**
    * Gives access to the specified texture associated with this material instance.
    *
    * @param textureUsage
    */
   inline Texture* getTexture( MaterialTextures textureUsage ) const { return m_texture[textureUsage]; }

   /**
    * Sets a new texture of the specified usage on the material instance.
    *
    * @param textureUsage
    * @param texture
    */
   inline void setTexture( MaterialTextures textureUsage, Texture* texture ) { m_texture[textureUsage] = texture; notify( MIO_MAT_CHANGED ); }

   /**
    * Returns the runtime data buffer.
    */
   inline RuntimeDataBuffer& data() const { return *m_dataBuf; }

   // -------------------------------------------------------------------------
   // Rendering support
   // -------------------------------------------------------------------------

   /**
    * Called before the geometry is rendered.
    *
    * @param renderer
    * @param entity     rendered entity
    */
   void preRender( Renderer& renderer, const MaterialEntity& entity ) const;

   /**
    * Called after the geometry is rendered.
    *
    * @param renderer
    * @param entity     rendered entity
    */
   void postRender( Renderer& renderer, const MaterialEntity& entity ) const;

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded( ResourcesManager& mgr );

   // -------------------------------------------------------------------------
   // Observer implementation
   // -------------------------------------------------------------------------
   void update( Material& subject );
   void update( Material& subject, const GraphBuilderOperation& msg );
   void update( MaterialNode& subject );
   void update( MaterialNode& subject, const GraphBuilderNodeOperation& msg );

private:
   void initializeMaterial();
   void deinitializeMaterial();
   void attachListeners();
   void detachListeners();
};

///////////////////////////////////////////////////////////////////////////////

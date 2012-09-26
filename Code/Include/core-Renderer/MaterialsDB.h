/// @file   core-Renderer/MaterialsDB.h
/// @brief  a database of all materials currently loaded and used in the engine
#pragma once

#include "core/types.h"
#include "core/Array.h"
#include "core/Vector.h"
#include "core/Observer.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialInstance;
enum MaterialInstanceOperation;

///////////////////////////////////////////////////////////////////////////////

/**
 * A database of all materials currently loaded and used in the engine.
 *
 * It's a singleton that manages absolutely all materials, providing an easy
 * access to all material instances used throughout the engine, and providing
 * all sorts of additional functionality related to materials management and rendering.
 */
class MaterialsDB : public Observer< MaterialInstance, MaterialInstanceOperation >
{
public:
   static uint                         RENDER_DATA_BUFFER_STRIDE;
   static uint                         TEXTURE_COORDS_STRIDE;

private:
   static MaterialsDB                  s_theInstance;

   Array< uint >                       m_materialHashes;
   Array< const MaterialInstance* >    m_materials;

   int                                 m_timestamp;

public:
   /**
    * Constructor ( public, cause we allow the creation of temporary databases for testing purposes ).
    */
   MaterialsDB();
   ~MaterialsDB();

   /**
    * Returns the singleton instance of the DB.
    */
   static inline MaterialsDB& getInstance() { return s_theInstance; }

   /**
    * Registers a new material with the database
    *
    * @param matInstance
    * @return     a unique ID assigned to this specific material instance
    */
   uint registerMaterial( MaterialInstance& matInstance );

   /**
    * Clears the database.
    */
   void clear();

   /**
    * Returns a timestamp that marks any changes introduced to the DB.
    */
   inline int getTimestamp() const { return m_timestamp; }

   /**
    * Notifies the database that one of its materials has changed.
    */
   inline void notifyAboutChange() { ++m_timestamp; }

   /**
    * Collects material data for rendering.
    *
    * Data will be stored in the specified array in vectors.
    * Each entry consists of a number of subsequent vectors, corresponding respectively to:
    * 1. ambient color
    * 2. diffuse color
    * 3. specular color ( rgb ) + specular power ( a )
    * 4. emissive color
    *
    * The number of those vectors is stored in RENDER_DATA_BUFFER_STRIDE member
    *
    * @param outDataBuffer
    */
   void collectMaterialData( Array< Vector >& outDataBuffer  ) const;

   
   /**
    * Generates a tightly packed coordinates for packing material textures in an atlas.
    *
    * Data will be stored in the specified array in vectors.
    * Each entry consists of a number of subsequent vectors, corresponding respectively to:
    * 1. diffuse 1 ( rg ) and diffuse 2 ( ba ) texture location in the atlas
    * 2. normal map ( rg ) and specular map ( ba ) location in the atlas
    *
    * The number of those vectors is stored in TEXTURE_COORDS_STRIDE member
    *
    * @param outTextureCoords
    */
   void generateTextureCoordinates( Array< Vector >& outTextureCoords ) const;

   // -------------------------------------------------------------------------
   // Observer implementation
   // -------------------------------------------------------------------------
   void update( MaterialInstance& subject );
   void update( MaterialInstance& subject, const MaterialInstanceOperation& msg );

private:
   /**
    * Copy constructor.
    *
    * @param rhs
    */
   MaterialsDB( const MaterialsDB& rhs );

   /**
    * Assignment operator.
    *
    * @param rhs
    */
   void operator=( const MaterialsDB& rhs );

   /**
    * Calculates a hash value for the material.
    *
    * @param matInstance
    */
   uint calcHash( const MaterialInstance& matInstance ) const;
};

///////////////////////////////////////////////////////////////////////////////

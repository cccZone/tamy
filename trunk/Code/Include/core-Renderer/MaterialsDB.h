/// @file   core-Renderer/MaterialsDB.h
/// @brief  a database of all materials currently loaded and used in the engine
#pragma once

#include "core/types.h"
#include "core/Array.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialInstance;

///////////////////////////////////////////////////////////////////////////////

/**
 * A database of all materials currently loaded and used in the engine.
 *
 * It's a singleton that manages absolutely all materials, providing an easy
 * access to all material instances used throughout the engine, and providing
 * all sorts of additional functionality related to materials management and rendering.
 */
class MaterialsDB
{
private:
   static MaterialsDB            s_theInstance;

   Array< uint >                 m_materialHashes;

public:
   /**
    * Constructor ( public, cause we allow the creation of temporary databases for testing purposes ).
    */
   MaterialsDB();

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
   uint registerMaterial( const MaterialInstance& matInstance );

   /**
    * Clears the database.
    */
   void clear();

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

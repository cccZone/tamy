/// @file   core-Renderer/Material.h
/// @brief  material resource
#pragma once

#include "core/Resource.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialEntity;
class Renderer;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

class Material : public Resource
{
   DECLARE_RESOURCE( Material )

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   Material( const FilePath& resourceName = FilePath() );
   ~Material();

   // -------------------------------------------------------------------------
   // Runtime functionality
   // -------------------------------------------------------------------------
   /**
    * Called by a MaterialEntity when it first starts using the Material.
    * Here's where the Material should initialize its runtime data structures.
    *
    * @param hostEntity
    * @param data
    */
   void initialize( MaterialEntity& hostEntity, RuntimeDataBuffer& data ) const;

   /**
    * Called by a MaterialEntity when it's about to stop using the Material.
    * Here's where the Material should deinitialize its runtime data structures.
    *
    * @param hostEntity
    * @param data
    */
   void deinitialize( MaterialEntity& hostEntity, RuntimeDataBuffer& data ) const;

   /**
    * Called in order to set material up before rendering.
    *
    * @param renderer
    * @param data
    */
   void onPreRender( Renderer& renderer, RuntimeDataBuffer& data ) const;
   
   /**
    * Called in order to clean up after the material after the rendering.
    *
    * @param renderer
    * @param data
    */
   void onPostRender( Renderer& renderer, RuntimeDataBuffer& data ) const;
};

///////////////////////////////////////////////////////////////////////////////

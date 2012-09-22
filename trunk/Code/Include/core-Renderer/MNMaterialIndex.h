/// @file   core-Renderer/MNMaterialIndex.h
/// @brief  supplies the index of the material instance the entity is using
#pragma once

#include "core-Renderer/MaterialNode.h"
#include "core-Renderer/MaterialEntity.h"


///////////////////////////////////////////////////////////////////////////////

class MSIntOutput;
class Texture;

///////////////////////////////////////////////////////////////////////////////

/**
 * supplies the index of the material instance the entity is using.
 */
class MNMaterialIndex : public MaterialNode
{
   DECLARE_CLASS()

private:
   // runtime data
   MSIntOutput*      m_index;
 
public:
   /**
    * Constructor.
    */
   MNMaterialIndex();
   ~MNMaterialIndex();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // MaterialNode implementation
   // -------------------------------------------------------------------------
   void preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////

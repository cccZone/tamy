/// @file   core-Renderer/MNSurfaceProperties.h
/// @brief  a node that provides surface properties set for the host material entity instance
#pragma once

#include "core-Renderer/MaterialNode.h"
#include "core-Renderer/MaterialEntity.h"


///////////////////////////////////////////////////////////////////////////////

class MSTextureOutput;
class Texture;

///////////////////////////////////////////////////////////////////////////////

class MNSurfaceProperties : public MaterialNode
{
   DECLARE_CLASS( MNSurfaceProperties )

private:
   // runtime data
   MSVec4Output*     m_ambient;
   MSVec4Output*     m_diffuse;
   MSVec4Output*     m_specular;
   MSFloatOutput*    m_specularPower;
   MSVec4Output*     m_emissive;
 
public:
   /**
    * Constructor.
    */
   MNSurfaceProperties();
   ~MNSurfaceProperties();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // MaterialNode implementation
   // -------------------------------------------------------------------------
   void preRender( Renderer& renderer, const MaterialEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////

#pragma once

/// @file   core-Scene\CameraEntity.h
/// @brief  a camera description

#include "core-Scene\Entity.h"
#include <d3dx9.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a directional light.
 */
struct CameraEntity : public Entity
{
   std::string m_name;
   D3DXMATRIX m_situation;

   /**
    * Default constructor.
    */
   CameraEntity();

   /**
    * Constructor.
    *
    * @param name
    * @param situation
    */
   CameraEntity(const std::string& name,
                const D3DXMATRIX& situation);

   // -------------------------------------------------------------------------
   // Serializable implementation
   // -------------------------------------------------------------------------
   void save(Serializer& serializer);

   void load(Serializer& serializer);
};

///////////////////////////////////////////////////////////////////////////////

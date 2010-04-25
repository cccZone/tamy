#pragma once

/// @file   core-Scene\CameraEntity.h
/// @brief  a camera description

#include "core-Scene\Entity.h"
#include "core-Scene\Situated.h"
#include <d3dx9.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a directional light.
 */
struct CameraEntity : public Entity, public Situated
{
public:
   std::string m_name;
   D3DXMATRIX m_situation;

public:
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
   // Situated implementation
   // -------------------------------------------------------------------------
   const D3DXMATRIX& getSituation() const {return m_situation;}

   // -------------------------------------------------------------------------
   // Serializable implementation
   // -------------------------------------------------------------------------
   void save(Serializer& serializer);

   void load(Serializer& serializer);

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
protected:
   void registerProperties();
};

///////////////////////////////////////////////////////////////////////////////

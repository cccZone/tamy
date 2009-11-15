#pragma once

/// @file   core-Scene\SkyBoxEntity.h
/// @brief  a sky box scenery description

#include "core-Scene\Entity.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a scenery made up of a simple sky box.
 */
struct SkyBoxEntity : public Entity
{
   std::string m_frontTex;
   std::string m_backTex;
   std::string m_leftTex;
   std::string m_rightTex;
   std::string m_topTex;
   std::string m_bottomTex;

   /**
    * Default constructor.
    */
   SkyBoxEntity();

   /**
    * Constructor.
    *
    * @param frontTex
    * @param backTex
    * @param leftTex
    * @param rightTex
    * @param topTex
    * @param bottomTex
    */
   SkyBoxEntity(const std::string& frontTex,
                const std::string& backTex,
                const std::string& leftTex,
                const std::string& rightTex,
                const std::string& topTex,
                const std::string& bottomTex);

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void save(Serializer& serializer);

   void load(Serializer& serializer);
};

///////////////////////////////////////////////////////////////////////////////

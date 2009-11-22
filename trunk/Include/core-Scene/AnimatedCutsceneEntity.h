#pragma once

/// @file   core-Scene\AnimatedCutsceneEntity.h
/// @brief  an entity with animated cutscene

#include "core-Scene\Situated.h"
#include "core-Scene\Entity.h"
#include <string>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes an animated cutscene - stuff with models etc.
 * that plays on its own and doesn't require any sophisticated AI control over
 * the character - all movement and logics is encoded in the animation within.
 */
struct AnimatedCutsceneEntity : public Entity, public Situated
{
public:
   std::string m_sceneFileName;
   std::string m_animationName;
   D3DXMATRIX m_situation;

   /// current time in the cutscene (with respect to its time line)
   float m_time;

public:

   /**
    * Default constructor.
    */
   AnimatedCutsceneEntity();

   /**
    * Constructor.
    */
   AnimatedCutsceneEntity(const std::string& sceneFileName,
                          const std::string& animationName,
                          const D3DXMATRIX& situation);

   // -------------------------------------------------------------------------
   // Situated implementation
   // -------------------------------------------------------------------------
   const D3DXMATRIX& getSituation() const {return m_situation;}

   // -------------------------------------------------------------------------
   // Entity implementation
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

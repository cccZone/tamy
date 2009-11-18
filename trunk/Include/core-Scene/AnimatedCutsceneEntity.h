#pragma once

/// @file   core-Scene\AnimatedCutsceneEntity.h
/// @brief  an entity with animated cutscene

#include "core-Scene\SpatiallyQueryable.h"
#include "core-Scene\Entity.h"
#include "core\BoundingSphere.h"
#include <string>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes an animated cutscene - stuff with models etc.
 * that plays on its own and doesn't require any sophisticated AI control over
 * the character - all movement and logics is encoded in the animation within.
 */
struct AnimatedCutsceneEntity : public Entity, public SpatiallyQueryable
{
public:
   std::string m_sceneFileName;
   std::string m_animationName;
   D3DXMATRIX m_situation;

   /// current time in the cutscene (with respect to its time line)
   float m_time;

private:
   BoundingSphere m_boundingVol;
   Array<Triangle*> m_noGeometry;

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
   // SpatiallyQueryable implementation
   // -------------------------------------------------------------------------
   const BoundingVolume& getBoundingVolume();

   const D3DXMATRIX& getGlobalMtx();

   const Array<Triangle*>& getBoundingGeometry();

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

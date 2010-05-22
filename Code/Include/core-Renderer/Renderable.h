#pragma once

/// @file   core-Renderer\Renderable.h
/// @brief  renderable 3d object

#include "core-Renderer\SpatialEntity.h"
#include <d3dx9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Geometry;
class Effect;
class EffectAttribute;

///////////////////////////////////////////////////////////////////////////////

/**
 * An atomic object that can be rendered Such an object consists of geometry
 * and effects that render the object in a particular way.
 */
class Renderable : public SpatialEntity
{
   DECLARE_CLASS(Renderable)

public:
   typedef std::vector<EffectAttribute*> Attributes;
   typedef std::vector<Effect*> Effects;

private:
   Geometry* m_geometry;
   Effects m_effects;

   // runtime data
   Attributes m_attributes;
   bool m_visible;

public:
   /**
    * Default constructor.
    */
   Renderable( const std::string& name = "" );
   ~Renderable();

   /**
    * Returns the list of all attributes of the renderable.
    * Attributes are taken from the effects the renderable 
    * contains.
    *
    * @return  effects attributes collection
    */
   const Attributes& getAttributes() const;

   /**
    * Returns effects used by the renderable.
    *
    * @return  collection of effects
    */
   Effects& getEffects();

   /**
    * Tells whether the geometry entity is set
    */
   bool hasGeometry() const;

   /**
    * Returns the geometry used by the renderable
    *
    * @return  underlying geometry
    */
   Geometry& getGeometry();

   /**
    * Changes visibility status of the renderable.
    *
    * @param visible    new visibility status
    */
   void setVisible(bool visible);

   /**
    * Returns current visibility status of the entity
    */
   bool isVisible() const;

protected:
   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void onChildAttached(Entity& child);
   void onChildDetached(Entity& child);

   // -------------------------------------------------------------------------
   // PropertyObserver implementation
   // -------------------------------------------------------------------------
   void onPropertyChanged(Property& property);

private:
   void removeEffect(Effect* effect);
   void addAttributes( Effect& effect );
};

///////////////////////////////////////////////////////////////////////////////

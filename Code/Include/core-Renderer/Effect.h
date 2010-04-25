#pragma once

/// @file   core-Renderer\Effect.h
/// @brief  rendering procedure

#include <vector>
#include "core-MVC\Entity.h"


///////////////////////////////////////////////////////////////////////////////

class Geometry;
class EffectAttribute;

///////////////////////////////////////////////////////////////////////////////

/**
 * An effect is a rendering procedure that sets renders
 * geometry in a specific way (maybe by applying a texture
 * to it, or maybe by rendering its shadows).
 */
class Effect : public Entity
{
   DECLARE_ABSTRACT_CLASS(Effect)

public:
   typedef std::vector<EffectAttribute*> Attributes;

private:
   Attributes* m_attributes;

public:
   virtual ~Effect();

   /**
    * Renders the geometry using this effect.
    *
    * @param geometry   geometry we wish to render using this effect
    */
   void render(Geometry& geometry);

   /**
    * Returns the collection of attributes this effect features.
    *
    * @return  effect attributes collection
    */
   const Attributes& getAttributes() const;

protected:
   /**
    * Constructor.
    */
   Effect();

   /**
    * This method is called when the effect starts being rendered.
    *
    * An effect can comprise multiple rendering passes - each adding
    * a new detail to the final rendering. This method should 
    * return the number of the passes this effect instance has.
    *
    * @return  number of rendering passes
    */
   virtual unsigned int beginRendering() = 0;

   /**
    * This method is called as a new rendering pass begins.
    *
    * @param passIdx    index of the pass that is about to commence
    */
   virtual void beginPass(unsigned int passIdx) = 0;

   /**
    * This method is called as a rendering pass ends.
    *
    * @param passIdx    index of the pass that has ended.
    */
   virtual void endPass(unsigned int passIdx) = 0;

   /**
    * This method is called once the effect ends all rendering.
    */
   virtual void endRendering() = 0;

   /**
    * This method allows to register an effect attribute
    * this effect features.
    * 
    * @param attribute  attribute we want to add
    */
   void addAttribute(EffectAttribute* attribute);

   // -------------------------------------------------------------------------
   // Entity default implementation
   // -------------------------------------------------------------------------
   void onSaveEntity(Serializer& serializer) {}
   void onLoadEntity(Serializer& serializer) {}
   void onChildAttached(Entity& child) {}
   void onChildDetached(Entity& child) {}
   void onAttached(Entity& parent) {}
   void onDetached(Entity& parent) {}
   void onAttached(Model& hostModel) {}
   void onDetached(Model& hostModel) {}
};

///////////////////////////////////////////////////////////////////////////////

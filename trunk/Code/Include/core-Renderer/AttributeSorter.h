#pragma once

/// @file   core-Renderer\AttributeSorter.h
/// @brief  sorters of effect attributes

#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Renderable;
class EffectAttribute;

///////////////////////////////////////////////////////////////////////////////

/**
 * This interface marks objects that can be stored inside
 * the attribute sorter collection.
 */
class Attributed
{
public:
   typedef std::vector<EffectAttribute*> Attributes;

public:
   virtual ~Attributed() {}

   virtual const Attributes& getAttributes() const = 0;

   virtual void render() = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * An attribute sorter is a tool for sorting renderables with respect to
 * a particular effect attribute.
 */
class AttributeSorter
{
public:
   virtual ~AttributeSorter() {}

   /**
    * Adds a new renderable to the sorter.
    *
    * @param renderable
    */
   virtual void add(Attributed& renderable) = 0;

   /**
    * Removes a renderable from the sorter.
    *
    * @param renderable
    */
   virtual void remove(Attributed& renderable) = 0;

   /**
    * The method invokes rendering operation on the enclosed
    * renderables.
    */
   virtual void render() = 0;
};

///////////////////////////////////////////////////////////////////////////////

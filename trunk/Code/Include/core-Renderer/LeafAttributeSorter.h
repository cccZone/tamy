#ifndef _LEAF_ATTRIBUTE_SORTER_H
#define _LEAF_ATTRIBUTE_SORTER_H

/// @file   core-Renderer\LeafAttributeSorter.h
/// @brief  effect attribute abiding renderables sorter

#include "core-Renderer\AttributeSorter.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This comparator needs to be specialized with a type of element attribute.
 * Each renderable that contains this attribute on its list will
 * be added to this container and sorted according to the value of that 
 * attribute.
 *
 * Mind that a renderable can contain multiple attributes of the same type.
 * In that case the first one on the list will be taken into account!
 */
template <typename T>
class LeafAttributeSorter : public AttributeSorter
{
private:
   struct Comparable
   {
      T& attrib;
      Attributed& object;

      Comparable(T& _attrib, Attributed& _object)
         : attrib(_attrib), object(_object)
      {}
   };

   struct Comparator
   {
      bool operator()(const Comparable* lhs, const Comparable* rhs) const
      {
         return lhs->attrib < rhs->attrib;
      }
   };

private:
   typedef std::vector<Comparable*> ComparablesVec;
   ComparablesVec m_comparables;

   Comparator m_comparator;

public:
   ~LeafAttributeSorter();

   // -------------------------------------------------------------------------
   // AttributeSorter implementation
   // -------------------------------------------------------------------------
   void add(Attributed& object);

   void remove(Attributed& object);

   void render();
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\LeafAttributeSorter.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _LEAF_ATTRIBUTE_SORTER_H

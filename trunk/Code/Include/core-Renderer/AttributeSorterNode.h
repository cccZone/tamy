#ifndef _ATTRIBUTE_SORTER_NODE_H
#define _ATTRIBUTE_SORTER_NODE_H

/// @file   core-Renderer\AttributeSorterNode.h
/// @brief  attributes sorting tree node

#include "core-Renderer\AttributeSorter.h"


///////////////////////////////////////////////////////////////////////////////

class AttributeSorterNode : public AttributeSorter
{
public:
   virtual ~AttributeSorterNode() {}

   /**
    * Replaces the sorter currently used to assign new objects to the 
    * top priority branch.
    * Renderables that were added to the previous sorter WILL NOT be copied 
    * over!
    * 
    * @param sorter     new attribute sorter
    */
   virtual void setPrimarySorter(AttributeSorter* sorter) = 0;

   /**
    * Replaces the sorter currently used to assign new objects to the 
    * second priority branch.
    * Renderables that were added to the previous sorter WILL NOT be copied 
    * over!
    * 
    * @param sorter     new attribute sorter
    */
   virtual void setSecondarySorter(AttributeSorter* sorter) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This sorter is designed to allow the creation of sorting binary trees.
 *
 * It works with a specific type of effect attribute, but doesn't
 * sort according to it - instead it checks if a renderable contains
 * an attribute of this type. If it does - the renderable is added to
 * to the top priority subtree, if not - it goes to the secondary priority subtree.
 *
 * The rendering of the tree is performed in the order:
 *    1.) top priority subtree
 *    2.) secondary priority subtree
 */
template <typename T>
class TAttributeSorterNode : public AttributeSorterNode
{
private:
   AttributeSorter* m_topPriorityChild;
   AttributeSorter* m_secondaryPriorityChild;

public:
   /**
    * Constructor.
    */
   TAttributeSorterNode();
   ~TAttributeSorterNode();

   // -------------------------------------------------------------------------
   // AttributeSorterNode implementation
   // -------------------------------------------------------------------------
   void setPrimarySorter(AttributeSorter* sorter);

   void setSecondarySorter(AttributeSorter* sorter);

   // -------------------------------------------------------------------------
   // AttributeSorter implementation
   // -------------------------------------------------------------------------
   void add(Attributed& object);

   void remove(Attributed& object);

   void render();
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\AttributeSorterNode.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _ATTRIBUTE_SORTER_NODE_H

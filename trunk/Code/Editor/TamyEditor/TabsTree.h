/// @file   TamyEditor/TabsTree.h
/// @brief  tree of the tab editors and splitters they are embedded in
#pragma once

#include <list>
#include "core/Delegate.h"


///////////////////////////////////////////////////////////////////////////////

class LeafTabWidget;
class QWidget;

///////////////////////////////////////////////////////////////////////////////

typedef Delegate< void ( LeafTabWidget* ) >   TabsTreeOperation;

///////////////////////////////////////////////////////////////////////////////

class TreeTabNode
{
public:
   virtual ~TreeTabNode() {}

   /**
    * If the node is a leaf node, return the instance to self with a proper type.
    */
   virtual LeafTabWidget* asLeaf() = 0;

   /**
    * Returns a pointer to self as a widget ( 'cause in the end it has to be a widget - either a splitter of a tabs manager ).
    */
   virtual QWidget* asWidget() = 0;

   /**
    * Collects the node's children.
    */
   virtual void collectChildren( std::list< TreeTabNode* >& outChildren ) = 0;

   /**
    * Runs an operation on each of the tree's leaves.
    *
    * @param rootNode
    * @param operation
    */
   static void executeBFS( TreeTabNode* rootNode, TabsTreeOperation& operation );
};

///////////////////////////////////////////////////////////////////////////////

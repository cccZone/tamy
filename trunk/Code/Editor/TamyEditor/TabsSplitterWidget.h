/// @file   TamyEditor/TabsSplitterWidget.h
/// @brief  a custom splitter acting as a composite node in the tabs tree
#pragma once

#include <QSplitter>
#include "TabsTree.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A custom splitter acting as a composite node in the tabs tree.
 */
class TabsSplitterWidget : public QSplitter, public TreeTabNode
{
   Q_OBJECT

public:
   /**
    * Constructor.
    * 
    * @param splitOrientation
    * @param parentWidget
    */
   TabsSplitterWidget( Qt::Orientation splitOrientation, QWidget* parentWidget );

   // -------------------------------------------------------------------------
   // TreeTabNode implementation
   // -------------------------------------------------------------------------
   LeafTabWidget* asLeaf() { return NULL; }
   QWidget* asWidget() { return this;}
   void collectChildren( std::list< TreeTabNode* >& outChildren );
};

///////////////////////////////////////////////////////////////////////////////

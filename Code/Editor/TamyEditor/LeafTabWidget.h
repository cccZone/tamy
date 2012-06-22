/// @file   TamyEditor/LeafTabWidget.h
/// @brief  a leaf of the tab widgets tree
#pragma once

#include <QTabWidget>
#include "TabsTree.h"


///////////////////////////////////////////////////////////////////////////////

class SplittableTabWidget;
class TabsSplitterWidget;

///////////////////////////////////////////////////////////////////////////////

class LeafTabWidget : public QTabWidget, public TreeTabNode
{
   Q_OBJECT

private:
   SplittableTabWidget&       m_treeManager;

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    * @param treeManager
    */
   LeafTabWidget( QWidget* parentWidget, SplittableTabWidget& treeManager );

   // -------------------------------------------------------------------------
   // TreeTabNode implementation
   // -------------------------------------------------------------------------
   LeafTabWidget* asLeaf() { return this; }
   QWidget* asWidget() { return this;}
   void collectChildren( std::list< TreeTabNode* >& outChildren ) {}

public slots:
   void closeTab( int tabIdx );
   void showPopupMenu( const QPoint& pos );
   void splitHorizontally();
   void splitVertically();

private:
   void split( Qt::Orientation splitOrientation );
   TabsSplitterWidget* createNewSplitter( Qt::Orientation splitOrientation, TabsSplitterWidget* parentSplitter );
   LeafTabWidget* moveActiveTabToNewSubwindow( TabsSplitterWidget* parentSplitter );
   LeafTabWidget* moveActiveSubwindowToSplitter( TabsSplitterWidget* parentSplitter, TabsSplitterWidget* targetSplitter );
};

///////////////////////////////////////////////////////////////////////////////

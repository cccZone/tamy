#include "LeafTabWidget.h"
#include <QMenu>
#include <QAction>
#include <QSplitter>
#include <QHBoxLayout>
#include "SplittableTabWidget.h"
#include "TabsSplitterWidget.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

LeafTabWidget::LeafTabWidget( QWidget* parentWidget, SplittableTabWidget& treeManager )
   : QTabWidget( parentWidget )
   , m_treeManager( treeManager )
{
   setContextMenuPolicy( Qt::CustomContextMenu );

   setTabsClosable( true );
   setMovable( true );
   connect( this, SIGNAL( tabCloseRequested ( int ) ), this, SLOT( closeTab( int ) ) );
   connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( showPopupMenu( const QPoint& ) ) );
}

///////////////////////////////////////////////////////////////////////////////

void LeafTabWidget::showPopupMenu( const QPoint& pos )
{
   uint tabsCount = count();
   if ( tabsCount < 2 )
   {
      // there's no point in splitting the window if it's just got one tab in it
      return;
   }

   // activate this widget to specify that the split out elements should end up as its children
   m_treeManager.setActiveTabWidget( this );

   // create the menu
   QMenu* popupMenu = new QMenu( this );
   {
      QAction* horizontalSplitAction = new QAction( "Split horizontally", this );
      connect( horizontalSplitAction, SIGNAL( triggered() ), this, SLOT( splitHorizontally() ) );
      popupMenu->addAction( horizontalSplitAction );

      QAction* verticalSplitAction = new QAction( "Split vertically", this );
      connect( verticalSplitAction, SIGNAL( triggered() ), this, SLOT( splitVertically() ) );
      popupMenu->addAction( verticalSplitAction );
   }

   // display the menu
   popupMenu->popup( mapToGlobal( pos ) );
}

///////////////////////////////////////////////////////////////////////////////

void LeafTabWidget::split( Qt::Orientation splitOrientation )
{
   TabsSplitterWidget* parentSplitter = static_cast< TabsSplitterWidget* >( this->parentWidget() );

   TabsSplitterWidget* targetSplitter = NULL;
   LeafTabWidget* newActiveTabWidget = NULL;
   if ( parentSplitter->orientation() != splitOrientation )
   { 
      if ( parentSplitter->count() == 1 )
      {
         // ok - parent splitter's not split yet, so simply change its orientation
         parentSplitter->setOrientation( splitOrientation );
         newActiveTabWidget = moveActiveTabToNewSubwindow( parentSplitter );
      }
      else
      {
         // we need to introduce a child splitter
         TabsSplitterWidget* targetSplitter = createNewSplitter( splitOrientation, parentSplitter );
         newActiveTabWidget = moveActiveSubwindowToSplitter( parentSplitter, targetSplitter );
      }
   }
   else
   {
      // orientation remains the same - simply extract the item to a new sub-window
      targetSplitter = parentSplitter;
      newActiveTabWidget = moveActiveTabToNewSubwindow( parentSplitter );
   }

   // set the new active tab
   m_treeManager.setActiveTabWidget( newActiveTabWidget );

}

///////////////////////////////////////////////////////////////////////////////

TabsSplitterWidget* LeafTabWidget::createNewSplitter( Qt::Orientation splitOrientation, TabsSplitterWidget* parentSplitter )
{
   // setup the new splitter
   TabsSplitterWidget* newSplitter = new TabsSplitterWidget( splitOrientation, NULL );

   // set the size of the new splitter
   {
      QSize splitterSize = parentSplitter->size();
      int subwindowSize = 0;
      switch( splitOrientation )
      {
      case Qt::Horizontal:
         {
            subwindowSize = splitterSize.width() / 2;
            splitterSize.setWidth( subwindowSize );
            break;
         }

      case Qt::Vertical:
         {
            subwindowSize = splitterSize.height() / 2;
            splitterSize.setHeight( subwindowSize );
            break;
         }
      }
      newSplitter->setBaseSize( splitterSize );
      QList< int > splitterSizes;
      splitterSizes.push_back( subwindowSize );
      newSplitter->setSizes( splitterSizes );
   }

   return newSplitter;
}

///////////////////////////////////////////////////////////////////////////////

LeafTabWidget* LeafTabWidget::moveActiveTabToNewSubwindow( TabsSplitterWidget* parentSplitter )
{
   // memorize the sizes of the parent splitter, 'cause after all the shuffling's done we're gonna want to restore them
   QList< int > parentSplitterSizes = parentSplitter->sizes();

   // extract currently active tab
   QIcon extractedPageIcon;
   QString extractedPageLabel;
   QWidget* extractedPageWidget = NULL;
   {
      int currentTabIdx = currentIndex();
      extractedPageIcon = tabIcon( currentTabIdx );
      extractedPageLabel = tabText( currentTabIdx );

      extractedPageWidget = currentWidget();
      removeTab( currentTabIdx );
   }

   // adjust sizes for the parent splitter
   {
      // first - identify the sub-window that's about to be split and 
      // split its size as well
      uint subwindowsCount = parentSplitter->count();
      uint splitSubwindowIdx = 0;
      for ( uint i = 0; i < subwindowsCount; ++i )
      {
         if ( parentSplitter->widget(i) == this )
         {
            // got it
            splitSubwindowIdx = i;
            break;
         }
      }

      uint newSize = parentSplitterSizes[splitSubwindowIdx] / 2;
      parentSplitterSizes[splitSubwindowIdx] = newSize;
      parentSplitterSizes.insert( parentSplitterSizes.begin() + splitSubwindowIdx, newSize );
   }

   // add the extracted widget to the target splitter
   LeafTabWidget* newTabWidget = NULL;
   {
      // create a new SplittableTabWidget in the new splitter
      newTabWidget = new LeafTabWidget( parentSplitter, m_treeManager );
      parentSplitter->addWidget( newTabWidget );

      // and move currently active editor to it
      extractedPageWidget->setParent( newTabWidget );
      int newTabIdx = newTabWidget->addTab( extractedPageWidget, extractedPageIcon, extractedPageLabel );
      newTabWidget->setCurrentIndex( newTabIdx );
   }

   // restore parent splitter sizes
   parentSplitter->setSizes( parentSplitterSizes );

   return newTabWidget;
}

///////////////////////////////////////////////////////////////////////////////

LeafTabWidget* LeafTabWidget::moveActiveSubwindowToSplitter( TabsSplitterWidget* parentSplitter, TabsSplitterWidget* targetSplitter )
{
   // Find the leaf widget in the parent splitter and substitute it for the target splitter.
   uint count = parentSplitter->count();
   uint currentLeafIdx = 0;
   for ( uint i = 0; i < count; ++i )
   {
      if ( parentSplitter->widget( i ) == this )
      {
         // found it
         currentLeafIdx = i;
         break;
      }
   }

   // Then - add the extracted leaf to the target splitter
   this->setParent( targetSplitter );
   targetSplitter->addWidget( this );
   parentSplitter->insertWidget( currentLeafIdx, targetSplitter );

   // and last but not least - extract the active tab to a new sub-window of the target splitter
   LeafTabWidget* newTabWidget = moveActiveTabToNewSubwindow( targetSplitter );

   return newTabWidget;
}

///////////////////////////////////////////////////////////////////////////////

void LeafTabWidget::closeTab( int tabIdx )
{
   // delete the selected tab's contents
   QWidget* widget = this->widget( tabIdx );
   m_treeManager.notifyTabClosed( widget );

   // collapse the splitter
   bool removeSelf = false;
   TabsSplitterWidget* hostSplitter = dynamic_cast< TabsSplitterWidget* >( this->parentWidget() );
   {
      uint remainingTabsCount = this->count();
      if ( remainingTabsCount == 0 )
      {
         QList< int > parentSplitterSizes = hostSplitter->sizes();

         // remove the empty sub-window
         uint subwindowsCount = hostSplitter->count();
         uint emptySubwindowIdx = 0;
         for ( uint i = 0; i < subwindowsCount; ++i )
         {
            if ( hostSplitter->widget( i )  == this )
            {
               // got it
               emptySubwindowIdx = i;
               break;
            }
         }

         if ( emptySubwindowIdx > 0 )
         {
            // enlarge the sub-window to the left
            parentSplitterSizes[emptySubwindowIdx-1] += parentSplitterSizes[emptySubwindowIdx];
         }
         else if ( emptySubwindowIdx + 1 < subwindowsCount )
         {
            // that's the first sub-window - so enlarge the one to the left
            parentSplitterSizes[emptySubwindowIdx+1] += parentSplitterSizes[emptySubwindowIdx];
         }
         parentSplitterSizes.erase( parentSplitterSizes.begin() + emptySubwindowIdx );

         // remove the empty sub-window
         removeSelf = true;
      }
   }

   // verify whether there are any tab managers left in this splitter
   LeafTabWidget* newActiveTabManager = this;
   bool removeHost = false;
   {
      if ( removeSelf )
      {
         this->setParent( NULL );
         this->deleteLater();
         newActiveTabManager = NULL;
      }

      uint parentSubwindowsCount = hostSplitter->count();
      if ( parentSubwindowsCount > 0 )
      {
         newActiveTabManager = static_cast< LeafTabWidget* >( hostSplitter->widget( parentSubwindowsCount - 1 ) );
         removeHost = false;
      }
      else
      {
         removeHost = true;
      }
   }

   TabsSplitterWidget* parentSplitter = dynamic_cast< TabsSplitterWidget* >( hostSplitter->parentWidget() );
   if ( !parentSplitter )
   {
      // remove self only if there's a parent node - if there's no parent node above, then leave it around
      removeHost = false;

      if ( !newActiveTabManager )
      {
         // Since this is the last remaining splitter, and we've just removed the last active tab manager in it,
         // we need to create a new one, because there can never be a situation when we're left without an active tab manager.
         newActiveTabManager = new LeafTabWidget( hostSplitter, m_treeManager );
      }
   }

   // now deal with a possible case of us removing the last sub-window of the child splitter
   if ( removeHost )
   {
      // select the parent splitter's last window as the active one{
      uint parentSubwindowsCount = parentSplitter->count();
      newActiveTabManager = static_cast< LeafTabWidget* >( parentSplitter->widget( parentSubwindowsCount - 1 ) );

      // remove the host splitter
      hostSplitter->setParent( NULL );
      delete hostSplitter;
   }

   // inform the top manager about a new tab widget it should activate
   ASSERT( newActiveTabManager != NULL );
   m_treeManager.setActiveTabWidget( newActiveTabManager );
}

///////////////////////////////////////////////////////////////////////////////

void LeafTabWidget::splitHorizontally()
{
   split( Qt::Horizontal );
}

///////////////////////////////////////////////////////////////////////////////

void LeafTabWidget::splitVertically()
{
   split( Qt::Vertical );
}

///////////////////////////////////////////////////////////////////////////////

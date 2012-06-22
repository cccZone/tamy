/// @file   TamyEditor/SplittableTabWidget.h
/// @brief  a frame will can dynamically split in half
#pragma once

#include <QFrame>
#include <vector>
#include "core/types.h"


///////////////////////////////////////////////////////////////////////////////

class QPoint;
class LeafTabWidget;
class SplittableTabWidget;
class TreeTabNode;
class QMouseEvent;

///////////////////////////////////////////////////////////////////////////////

class SplittableTabLocation
{
private:
   friend class SplittableTabWidget;

   LeafTabWidget*       m_tabManager;
   uint                 m_tabIdx;

public:
   SplittableTabLocation() : m_tabManager( NULL ), m_tabIdx( -1 ) {}
   SplittableTabLocation( LeafTabWidget* tabManager, uint tabIdx ) : m_tabManager( tabManager ), m_tabIdx( tabIdx ) {}

   inline bool exists() const { return m_tabManager != NULL; }
};

///////////////////////////////////////////////////////////////////////////////

class SplittableTabWidget : public QFrame
{
   Q_OBJECT

private:
   LeafTabWidget*       m_activeTabWidget;
   TreeTabNode*         m_rootNode;

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    */
   SplittableTabWidget( QWidget* parentWidget = NULL );

   /**
    * Sets the specified leaf tab widget as an active one ( one to which the new tabs will be added ).
    *
    * @param activeTabWidget
    */
   void setActiveTabWidget( LeafTabWidget* activeTabWidget ) { m_activeTabWidget = activeTabWidget; }

   /**
    * Activates the tab at the specified location.
    *
    * @param location
    */
   void setActiveTab( const SplittableTabLocation& location );

   /**
    * Adds a new tab to the active tab widget, embedding the specified widget in it and 
    * labeling it with the specified label and icon.
    *
    * @param widget
    * @param icon
    * @param label
    */
   void addTab( QWidget* widget, const QIcon& icon, QString& label );

   /**
    * Locates a tab in the tree with the specified name.
    *
    * @param tabName       name of the tab we're looking for
    * @param outLocation   location of the tab ( providing we found the tab of course )
    * @return              'true' if the tab was found, 'false' otherwise
    */
   bool findTabByName( const QString& tabName, SplittableTabLocation& outLocation ) const;


   /**
    * Collects all tab widgets embedded in the tree.
    *
    * @param outTabWidgets       widgets will be appended to this array
    * @param outTabsLocations    if you want to learn their exact locations in the tree, you can specify a pointer to this array
    */
   void collectPageWidgets( std::vector< QWidget* >& outTabWidgets, std::vector< SplittableTabLocation >* outTabsLocations = NULL ) const;

   /**
    * Removes the tab located under the specified location.
    *
    * @param location
    */
   void removeTab( const SplittableTabLocation& location );

   /**
    * Removes all tabs ( doesn't destroy the embedded widgets though ).
    */
   void clear();

signals:
   /**
    * A signal emitted when one of the tabs is closed.
    *
    * @param widget.
    */
   void onTabClosed( QWidget* widget );

protected:
   friend class LeafTabWidget;
   void notifyTabClosed( QWidget* widget );
   void createNewRoot();
};

///////////////////////////////////////////////////////////////////////////////

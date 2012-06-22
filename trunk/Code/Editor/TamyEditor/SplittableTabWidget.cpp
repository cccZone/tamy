#include "SplittableTabWidget.h"
#include <QHBoxLayout>
#include <QMouseEvent>
#include "LeafTabWidget.h"
#include "TabsSplitterWidget.h"
#include "TabsTree.h"


// <editor.todo> !!!!!!!!!!!!!!! moving tabs between tab managers
// <editor.todo> !!!!!!!!!!!!!!! document this widget


///////////////////////////////////////////////////////////////////////////////

SplittableTabWidget::SplittableTabWidget( QWidget* parentWidget )
   : QFrame( parentWidget )
{
   // set the frame layout
   QHBoxLayout* layout = new QHBoxLayout( this );
   layout->setSpacing( 0 );
   layout->setMargin( 0 );
   setLayout( layout );

   // add the root tab widget
   TabsSplitterWidget* rootSplitter = new TabsSplitterWidget( Qt::Horizontal, this );
   m_rootNode = rootSplitter;
   layout->addWidget( rootSplitter );

   m_activeTabWidget = new LeafTabWidget( rootSplitter, *this );
   rootSplitter->addWidget( m_activeTabWidget );
}

///////////////////////////////////////////////////////////////////////////////

void SplittableTabWidget::setActiveTab( const SplittableTabLocation& location )
{
   if ( location.m_tabManager == NULL )
   {
      return;
   }

   m_activeTabWidget = location.m_tabManager;
   m_activeTabWidget->setCurrentIndex( location.m_tabIdx );
}

///////////////////////////////////////////////////////////////////////////////

void SplittableTabWidget::addTab( QWidget* widget, const QIcon& icon, QString& label )
{
   int addedTabIdx = m_activeTabWidget->addTab( widget, icon, label );
   m_activeTabWidget->setCurrentIndex( addedTabIdx );
}

///////////////////////////////////////////////////////////////////////////////


bool SplittableTabWidget::findTabByName( const QString& tabName, SplittableTabLocation& outLocation ) const
{
   struct TabsOperation
   {
      QString                    m_tabName;
      SplittableTabLocation&     m_outTabsLocation;

      TabsOperation( const QString& tabName, SplittableTabLocation& outTabsLocation )
         : m_tabName( tabName )
         , m_outTabsLocation( outTabsLocation )
      {
      }

      void findTabByName( LeafTabWidget* tabsManager )
      {
         uint count = tabsManager->count();
         for ( uint i = 0; i < count; ++i )
         {
            QString tabLabel = tabsManager->tabText(i);
            if ( tabLabel == m_tabName )
            {
               // found it
               m_outTabsLocation.m_tabManager = tabsManager;
               m_outTabsLocation.m_tabIdx = i;

               // search no more
               break;
            }
         }
      }
   };

   TabsOperation operationDef( tabName, outLocation );
   TabsTreeOperation operation = TabsTreeOperation::FROM_METHOD( TabsOperation, findTabByName, &operationDef );
   TreeTabNode::executeBFS( m_rootNode, operation );

   return outLocation.exists();
}

///////////////////////////////////////////////////////////////////////////////

void SplittableTabWidget::collectPageWidgets( std::vector< QWidget* >& outTabWidgets, std::vector< SplittableTabLocation >* outTabsLocations ) const
{
   struct TabsOperation
   {
      std::vector< QWidget* >&                  m_outTabWidgets;
      std::vector< SplittableTabLocation >*     m_outTabsLocations;

      TabsOperation( std::vector< QWidget* >& outTabWidgets, std::vector< SplittableTabLocation >* outTabsLocations )
         : m_outTabWidgets( outTabWidgets )
         , m_outTabsLocations( outTabsLocations )
      {
      }

      void collectPageWidgets( LeafTabWidget* tabsManager )
      {
         uint count = tabsManager->count();
         for ( uint i = 0; i < count; ++i )
         {
            QWidget* widget = tabsManager->widget(i);
            m_outTabWidgets.push_back( widget );

            if ( m_outTabsLocations )
            {
               m_outTabsLocations->push_back( SplittableTabLocation( tabsManager, i ) );
            }
         }
      }
   };

   TabsOperation operationDef( outTabWidgets, outTabsLocations );
   TabsTreeOperation operation = TabsTreeOperation::FROM_METHOD( TabsOperation, collectPageWidgets, &operationDef );
   TreeTabNode::executeBFS( m_rootNode, operation );
}

///////////////////////////////////////////////////////////////////////////////

void SplittableTabWidget::removeTab( const SplittableTabLocation& location )
{
   if ( location.exists() )
   {
      location.m_tabManager->removeTab( location.m_tabIdx );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SplittableTabWidget::clear()
{
   struct TabsOperation
   {
      std::vector< LeafTabWidget* >&             m_outTabManagers;

      TabsOperation( std::vector< LeafTabWidget* >& outTabManagers )
         : m_outTabManagers( outTabManagers )
      {
      }

      void collectTabManagers( LeafTabWidget* tabsManager )
      {
         m_outTabManagers.push_back( tabsManager );
      }
   };

   // collect all children tab managers
   std::vector< LeafTabWidget* > outTabManagers;
   TabsOperation operationDef( outTabManagers );
   TabsTreeOperation operation = TabsTreeOperation::FROM_METHOD( TabsOperation, collectTabManagers, &operationDef );
   TreeTabNode::executeBFS( m_rootNode, operation );

   // remove the root widget from the tree
   layout()->removeWidget( m_rootNode->asWidget() );

   // clear their contents
   uint count = outTabManagers.size();
   for ( uint i = 0; i < count; ++i )
   {
      outTabManagers[i]->clear();
   }
   outTabManagers.clear();

   // create the new structure
   m_activeTabWidget = new LeafTabWidget( this, *this );
   m_rootNode = m_activeTabWidget;
   layout()->addWidget( m_activeTabWidget );
}

///////////////////////////////////////////////////////////////////////////////

void SplittableTabWidget::notifyTabClosed( QWidget* widget )
{
   emit onTabClosed( widget );
}

///////////////////////////////////////////////////////////////////////////////

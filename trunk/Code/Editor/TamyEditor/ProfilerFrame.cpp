#include "ProfilerFrame.h"
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSplitter>
#include <QScrollArea>
#include <QLabel>
#include "ProfilerChart.h"


///////////////////////////////////////////////////////////////////////////////

ProfilerFrame::ProfilerFrame()
   : m_callstackTree( NULL )
{
   // create the main window splitter
   QSplitter* windowSplitter = new QSplitter( Qt::Horizontal, this );
   {
      // setup the frame layout and add the splitter widget to it
      QHBoxLayout* layout = new QHBoxLayout( this );
      layout->setMargin( 0 );
      layout->setSpacing( 0 );
      setLayout( layout );

      layout->addWidget( windowSplitter );
   }

   // create a tree widget that will display a call-stack of profiled methods
   m_callstackTree = new QTreeWidget( windowSplitter );
   {
      m_callstackTree->setColumnCount( 2 );
      QStringList headerLabels;
      headerLabels << "Timer ID" << "Name";
      m_callstackTree->setHeaderLabels( headerLabels );

      connect( m_callstackTree, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( showSelectedTimerProfile( QTreeWidgetItem*, int ) ) );

      windowSplitter->addWidget( m_callstackTree );
   }

   // create a frame where we'll put the charts
   {
      QScrollArea* chartsFrameScrollArea = new QScrollArea( windowSplitter );
      chartsFrameScrollArea->setWidgetResizable( true );
      windowSplitter->addWidget( chartsFrameScrollArea );

      QFrame* chartsFrame = new QFrame( chartsFrameScrollArea );
      chartsFrameScrollArea->setWidget( chartsFrame );

      m_chartsLayout = new QFormLayout( chartsFrame );
      chartsFrame->setLayout( m_chartsLayout );
   }

   // configure the splitter
   {
      windowSplitter->setOpaqueResize( false );

      QList< int > splitterWindowSizes;
      splitterWindowSizes.push_back( width() / 2 );
      splitterWindowSizes.push_back( width() / 2 );
      windowSplitter->setSizes( splitterWindowSizes );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ProfilerFrame::onInitialize()
{
   // nothing to do here
}

///////////////////////////////////////////////////////////////////////////////

void ProfilerFrame::onDeinitialize( bool saveProgress )
{
   // nothing to do here
}

///////////////////////////////////////////////////////////////////////////////

uint ProfilerFrame::getTimerId( QTreeWidgetItem* item ) const
{
   uint itemTimerId = 0;
   if ( item != NULL )
   {
      itemTimerId = item->data( CALLSTACK_COL_TIMER_ID, 0 ).toInt();
   }
   return itemTimerId;
}

///////////////////////////////////////////////////////////////////////////////

void ProfilerFrame::update( float timeElapsed )
{
   const Profiler& profiler = Profiler::getInstance();

   QTreeWidgetItem* parentItem = NULL;
   QTreeWidgetItem* item = NULL;
   const Profiler::Trace* prevTrace = NULL;

   uint tracesCount = profiler.getTracesCount();
   for ( uint i = 0; i < tracesCount; ++i )
   {
      // step through subsequent traces, adding new tree leaves if required,
      // and updating the profiling information for each
      const Profiler::Trace& trace = profiler.getTrace( i );

      findCallstackItem( prevTrace, trace, parentItem, item );
      if ( item == NULL )
      {
         // an entry for this method doesn't exist yet - add it
         item = addCallstackItem( parentItem, profiler, trace );
      }

      // memorize the parent item and the parent trace id for future lookups
      prevTrace = &trace;
   }


   // update timers
   uint chartsCount = m_charts.size();
   for ( uint i = 0; i < chartsCount; ++i )
   {
      ProfilerChart* chart = m_charts[i].m_chart;
      QLabel* topValueLabel = m_charts[i].m_topValueLabel;


      double timerTimeElapsed = profiler.getTimeElapsed( chart->m_timerId );
      float topValue = chart->addSample( timeElapsed, timerTimeElapsed );

      QString topValueText;
      topValueText.sprintf( "%.3f [us]", topValue * 1000.0f );
      topValueLabel->setText( topValueText );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ProfilerFrame::findCallstackItem( const Profiler::Trace* prevTrace, const Profiler::Trace& trace, QTreeWidgetItem*& parentItem, QTreeWidgetItem*& item ) const
{
   QTreeWidgetItem* analyzedItem = NULL;

   if ( !prevTrace )
   {
      // this is the first trace - see if any of the root items matches the parent trace id
      int count = m_callstackTree->topLevelItemCount();
      for( int i = 0; i < count; ++i )
      {
         QTreeWidgetItem* childItem = m_callstackTree->topLevelItem( i );
         uint itemTraceId = getTimerId( childItem );
         if ( itemTraceId == trace.m_parentTimerId )
         {
            // found it
            analyzedItem = childItem;
            break;
         }
      }
   }
   else
   {
      if ( prevTrace->m_timerId == trace.m_parentTimerId )
      {
         // this is a method call made from the previous method, so we're going deeper into the call-stack
         analyzedItem = item;
      }
      else if ( prevTrace->m_parentTimerId == trace.m_parentTimerId )
      {
         // this is a method call made from the same context the previous method call was made ( they share the same parent trace )
         analyzedItem = parentItem;
      }
      else if ( parentItem )
      {
         // this is a call made from higher up in the call stack, so we need to find the parent item that
         // bears the new trace's parent's id
         for ( analyzedItem = parentItem->parent(); analyzedItem != NULL; analyzedItem = parentItem->parent() )
         {
            uint itemTraceId = getTimerId( analyzedItem );
            if ( itemTraceId == trace.m_parentTimerId )
            {
               // found it
               break;
            }
         }
      }
   }

   // memorize the new parent item
   parentItem = analyzedItem;
   item = NULL;

   // find the corresponding trace item
   if ( analyzedItem )
   {
      int childrenCount = analyzedItem->childCount();
      for ( int i = 0; i < childrenCount; ++i )
      {
         QTreeWidgetItem* childItem = analyzedItem->child( i );

         uint itemTraceId = getTimerId( childItem );
         if ( itemTraceId == trace.m_timerId )
         {
            // found it
            item = childItem;
            return;
         }
      }
   }
   else
   {
      // look in the tree root
      int count = m_callstackTree->topLevelItemCount();
      for( int i = 0; i < count; ++i )
      {
         QTreeWidgetItem* childItem = m_callstackTree->topLevelItem( i );
         uint itemTraceId = getTimerId( childItem );
         if ( itemTraceId == trace.m_timerId )
         {
            // found it
            item = childItem;
            break;
         }
      }
   }

   // if we found ourselves here, it means that it's just a trace that doesn't have a corresponding item created yet
}

///////////////////////////////////////////////////////////////////////////////

QTreeWidgetItem* ProfilerFrame::addCallstackItem( QTreeWidgetItem* parentItem, const Profiler& profiler, const Profiler::Trace& trace )
{
   QTreeWidgetItem* newItem = NULL;

   if ( !parentItem )
   {
      // this is a top level tree item
      newItem = new QTreeWidgetItem( m_callstackTree );
      m_callstackTree->addTopLevelItem( newItem );
   }
   else
   {
      newItem = new QTreeWidgetItem( parentItem );
      parentItem->addChild( newItem );
   }

   // setup the new item
   const std::string& timerName = profiler.getName( trace.m_timerId );
   newItem->setText( CALLSTACK_COL_NAME, timerName.c_str() );
   newItem->setData( CALLSTACK_COL_TIMER_ID, 0, QVariant::fromValue< int >( trace.m_timerId ) );

   return newItem;
}

///////////////////////////////////////////////////////////////////////////////

void ProfilerFrame::showSelectedTimerProfile( QTreeWidgetItem* timerItem, int clickedColumnIdx )
{
   // check if we aren't already plotting a chart for this one
   uint timerId = getTimerId( timerItem );

   uint chartsCount = m_charts.size();
   for ( uint i = 0; i < chartsCount; ++i )
   {
      if ( m_charts[i].m_chart->m_timerId == timerId )
      {
         // yup - already plotting this one
         return;
      }
   }

   // we currently don't have a chart for this timer, so let's create it
   Profiler& profiler = Profiler::getInstance();

   QString chartLabel;
   chartLabel.sprintf( "%d ) %s:", timerId, profiler.getName( timerId ).c_str() );

   QFrame* chartFrame = new QFrame( m_chartsLayout->parentWidget() );
   {
      QHBoxLayout* chartFrameLayout = new QHBoxLayout( chartFrame );
      chartFrameLayout->setMargin( 0.0f );
      chartFrameLayout->setSpacing( 0.0f );
      chartFrame->setLayout( chartFrameLayout );

      ProfilerChart* chart = new ProfilerChart( m_chartsLayout->parentWidget(), timerId );
      chartFrameLayout->addWidget( chart );
   
      QLabel* topValueLabel = new QLabel( chartFrame );
      chartFrameLayout->addWidget( topValueLabel );

      m_charts.push_back( ChartDesc( chart, topValueLabel ) );
   }

   m_chartsLayout->addRow( chartLabel, chartFrame );
}

///////////////////////////////////////////////////////////////////////////////

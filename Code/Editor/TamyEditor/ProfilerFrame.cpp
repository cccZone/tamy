#include "ProfilerFrame.h"
#include <QTreeWidget>
#include <QHBoxLayout>


///////////////////////////////////////////////////////////////////////////////

ProfilerFrame::ProfilerFrame()
   : m_callstackTree( NULL )
{
   // setup the frame layout
   QHBoxLayout* layout = new QHBoxLayout( this );
   setLayout( layout );

   // create a tree widget that will display a call-stack of profiled methods
   m_callstackTree = new QTreeWidget( this );
   {
      m_callstackTree->setColumnCount( 2 );
      QStringList headerLabels;
      headerLabels << "Trace ID" << "Name";
      m_callstackTree->setHeaderLabels( headerLabels );
   }

   layout->addWidget( m_callstackTree );
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

void ProfilerFrame::update()
{
   const Profiler& profiler = Profiler::getInstance();

   QTreeWidgetItem* parentItem = NULL;
   QTreeWidgetItem* item = NULL;
   const Profiler::Trace* prevTrace = NULL;

   uint tracesCount = profiler.getCount();
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

      // update the profiling information in the item
      updateCallstackProfile( item, profiler, trace );

      // memorize the parent item and the parent trace id for future lookups
      prevTrace = &trace;
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
         uint itemTraceId = childItem->data( CALLSTACK_COL_TRACE_ID, 0 ).toInt();
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
            uint itemTraceId = analyzedItem->data( CALLSTACK_COL_TRACE_ID, 0 ).toInt();
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

         uint itemTraceId = childItem->data( CALLSTACK_COL_TRACE_ID, 0 ).toInt();
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
         uint itemTraceId = childItem->data( CALLSTACK_COL_TRACE_ID, 0 ).toInt();
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
   newItem->setData( CALLSTACK_COL_TRACE_ID, 0, QVariant::fromValue< int >( trace.m_timerId ) );

   return newItem;
}

///////////////////////////////////////////////////////////////////////////////

void ProfilerFrame::updateCallstackProfile( QTreeWidgetItem* item, const Profiler& profiler, const Profiler::Trace& trace )
{
}

///////////////////////////////////////////////////////////////////////////////

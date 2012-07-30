/// @file   TamyEditor/ProfilerFrame.h
/// @brief  a frame that shows the engine profiler results
#pragma once

#include "ResourceEditor.h"
#include "core/Profiler.h"


///////////////////////////////////////////////////////////////////////////////

class QTreeWidget;
class QTreeWidgetItem;

///////////////////////////////////////////////////////////////////////////////

/**
 * A frame that shows the engine profiler results.
 */
class ProfilerFrame : public ResourceEditor
{
   Q_OBJECT

private:
   enum CallstackTreeColumns
   {
      CALLSTACK_COL_TRACE_ID,
      CALLSTACK_COL_NAME,
   };

private:
   QTreeWidget*      m_callstackTree;

public:
   /**
    * Constructor.
    */
   ProfilerFrame();

   /**
    * Call this method to update profiling information.
    */
   void update();

   // -------------------------------------------------------------------------
   // ResourceEditor implementation
   // -------------------------------------------------------------------------
   void onInitialize();
   void onDeinitialize( bool saveProgress );

private:
   // callstack tree related methods
   void findCallstackItem( const Profiler::Trace* prevTrace, const Profiler::Trace& trace, QTreeWidgetItem*& parentItem, QTreeWidgetItem*& item ) const;
   QTreeWidgetItem* addCallstackItem( QTreeWidgetItem* parentItem, const Profiler& profiler, const Profiler::Trace& trace );
   void updateCallstackProfile( QTreeWidgetItem* item, const Profiler& profiler, const Profiler::Trace& trace );
};

///////////////////////////////////////////////////////////////////////////////

#include "DropFrame.h"
#include "MimeDataEncoder.h"
#include <QDragEnterEvent>
#include <QUrl>


///////////////////////////////////////////////////////////////////////////////

DropFrame::DropFrame( QWidget *parent, MimeDataEncoder* dataEncoder )
   : QFrame( parent )
   , m_dataEncoder( dataEncoder )
{
   setAcceptDrops( true );
}

///////////////////////////////////////////////////////////////////////////////

DropFrame::~DropFrame()
{
   delete m_dataEncoder;
   m_dataEncoder = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DropFrame::dragEnterEvent( QDragEnterEvent *event )
{
   setBackgroundRole( QPalette::Highlight );
   event->acceptProposedAction();
}

///////////////////////////////////////////////////////////////////////////////

void DropFrame::dragMoveEvent( QDragMoveEvent *event )
{
   event->acceptProposedAction();
}

///////////////////////////////////////////////////////////////////////////////

void DropFrame::dropEvent( QDropEvent *event )
{
   const QMimeData* mimeData = event->mimeData();

   m_dataEncoder->load( *mimeData );

   setBackgroundRole( QPalette::Light );
   event->acceptProposedAction();

   emit changed();
}

///////////////////////////////////////////////////////////////////////////////

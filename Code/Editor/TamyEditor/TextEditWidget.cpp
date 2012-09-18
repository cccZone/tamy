#include "TextEditWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QScrollBar>
#include <QPaintEvent>
#include <QPainter>
#include "TextSyntaxHighlighter.h"


///////////////////////////////////////////////////////////////////////////////

#define LINE_NUMBERS_AREA_WIDTH  40

///////////////////////////////////////////////////////////////////////////////

TextEditWidget::TextEditWidget( QWidget* parentWidget )
   : QFrame( parentWidget )
   , m_highlighter( NULL )
{
   QVBoxLayout* mainLayout = new QVBoxLayout( this );
   mainLayout->setMargin( 0 );
   mainLayout->setSpacing( 1 );
   setLayout( mainLayout );

   // create the main text editor are that will become a host to a list of lines and the text editor
   QFrame* textEditFrame = new QFrame( this );
   {
      mainLayout->addWidget( textEditFrame, 1 );

      QHBoxLayout* textEditLayout = new QHBoxLayout( textEditFrame );
      textEditLayout->setMargin( 0 );
      textEditLayout->setSpacing( 1 );
      textEditFrame->setLayout( textEditLayout );

      // setup the lines list widget
      {
         m_lineNumbers = new LinesWidget( textEditFrame );
         textEditLayout->addWidget( m_lineNumbers, 0, Qt::AlignLeft );
      }

      // setup the text editor widget
      {
         m_editor = new CodeEditorWidget( textEditFrame );
         textEditLayout->addWidget( m_editor, 1 );

         m_editor->setLineWrapMode( QPlainTextEdit::NoWrap );
         connect( m_editor, SIGNAL( textChanged() ), this, SLOT( onTextChanged() ) );
         connect( m_editor, SIGNAL( cursorPositionChanged() ), this, SLOT( onTextCursorMoved() ) );
         connect( m_editor, SIGNAL( selectionChanged() ), this, SLOT( onSelectionChanged() ) );
         connect( m_editor, SIGNAL( updateRequest( QRect,int ) ), m_lineNumbers, SLOT( updateArea( QRect,int ) ) );

         // make sure we synchronize the scrollbars between the lines list and the editor
         QScrollBar* editorScrollBar = m_editor->verticalScrollBar();
         connect( editorScrollBar, SIGNAL( valueChanged( int ) ), this, SLOT( documentScrolled( int ) ) );

         // setup the syntax highlighter
         setSyntaxHighlighter( new TextSyntaxHighlighter() );
      }

      // inform the line numbers widget about the editor
      m_lineNumbers->setTextEditor( m_editor );
   }

   // add a status bar underneath the main editor are
   m_statusBar = new QLabel( this );
   mainLayout->addWidget( m_statusBar, 0, Qt::AlignBottom );
}

///////////////////////////////////////////////////////////////////////////////

TextEditWidget::~TextEditWidget()
{
   delete m_highlighter;
   m_highlighter = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TextEditWidget::setSyntaxHighlighter( TextSyntaxHighlighter* highlighter )
{
   delete m_highlighter;
   m_highlighter = highlighter;

   if ( !m_highlighter )
   {
      m_highlighter = new TextSyntaxHighlighter();
   }

   m_highlighter->setDocument( m_editor->document() );
}

///////////////////////////////////////////////////////////////////////////////

void TextEditWidget::setFont( const QFont& font )
{
   QFontMetrics metrics( font );
   float tabWidth = metrics.width( "\t" ) * 0.5f;

   m_editor->setTabStopWidth( tabWidth );
   m_editor->setFont( font );

   int lineHeight = metrics.height();
   m_lineNumbers->setFont( font );
}

///////////////////////////////////////////////////////////////////////////////

void TextEditWidget::setPlainText( const QString& text )
{
   m_editor->setPlainText( text );
}

///////////////////////////////////////////////////////////////////////////////

QString TextEditWidget::toPlainText() const
{
   return m_editor->toPlainText();
}

///////////////////////////////////////////////////////////////////////////////

void TextEditWidget::onTextChanged()
{
   emit textChanged();
}

///////////////////////////////////////////////////////////////////////////////

void TextEditWidget::onTextCursorMoved()
{
   // write the line we're in
   QTextCursor cursor = m_editor->textCursor();
   
   int lineNumber = cursor.blockNumber() + 1;

   char tmpStr[256];
   sprintf( tmpStr, "Row: %d, Col: %d", lineNumber, cursor.columnNumber() );
   m_statusBar->setText( tmpStr );

   // highlight the line we're currently editing
   highlightCurrentLine();
}

///////////////////////////////////////////////////////////////////////////////

void TextEditWidget::onSelectionChanged()
{
   if ( !m_highlighter )
   {
      return;
   }
   // highlight all instances of the selected text
   QTextCursor cursor = m_editor->textCursor();
   int cursorPos = cursor.position();
   cursor.select( QTextCursor::WordUnderCursor );
   cursor.setPosition( cursorPos );
   QString selectedText = cursor.selectedText();

   m_highlighter->highlightAllInstances( selectedText );
}

///////////////////////////////////////////////////////////////////////////////

void TextEditWidget::undo()
{
   m_editor->undo();
}

///////////////////////////////////////////////////////////////////////////////

void TextEditWidget::redo()
{
   m_editor->redo();
}

///////////////////////////////////////////////////////////////////////////////

void TextEditWidget::documentScrolled( int scrollValue )
{
}

///////////////////////////////////////////////////////////////////////////////

void TextEditWidget::highlightCurrentLine()
{
   QList< QTextEdit::ExtraSelection > extraSelections;

   QTextEdit::ExtraSelection selection;
   QColor lineColor = QColor( 250, 200, 200 );

   selection.format.setBackground(lineColor);
   selection.format.setProperty(QTextFormat::FullWidthSelection, true);
   selection.cursor = m_editor->textCursor();
   selection.cursor.clearSelection();
   extraSelections.append(selection);

   m_editor->setExtraSelections( extraSelections );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CodeEditorWidget::CodeEditorWidget( QWidget* parentWidget )
   : QPlainTextEdit( parentWidget )
{
}

///////////////////////////////////////////////////////////////////////////////

void CodeEditorWidget::paintLines( LinesWidget* linesWidget, QPaintEvent* event )
{
   QPainter painter( linesWidget );
   painter.fillRect( event->rect(), Qt::lightGray );

   //The lineNumberAreaPaintEvent() is called from LineNumberArea whenever it receives a paint event. We start off by painting the widget's background.

   QTextBlock block = firstVisibleBlock();
   int blockNumber = block.blockNumber();
   int top = (int)blockBoundingGeometry(block).translated( contentOffset() ).top();
   int bottom = top + (int)blockBoundingRect( block ).height();

   // We will now loop through all visible lines and paint the line numbers in the extra area for each line. 
   // Notice that in a plain text edit each line will consist of one QTextBlock; though, if line wrapping is enabled, a line may span several rows in the text edit's viewport.
   // We get the top and bottom y-coordinate of the first text block, and adjust these values by the height of the current text block in each iteration in the loop.

   while( block.isValid() && top <= event->rect().bottom() ) 
   {
      if ( block.isVisible() && bottom >= event->rect().top() ) 
      {
         QString number = QString::number( blockNumber + 1 );
         painter.setPen( Qt::black );
         painter.drawText( 0, top, linesWidget->width(), fontMetrics().height(), Qt::AlignRight, number );
      }

      block = block.next();
      top = bottom;
      bottom = top + (int)blockBoundingRect( block ).height();
      ++blockNumber;
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

LinesWidget::LinesWidget( QWidget* parentWidget )
   : QWidget( parentWidget )
   , m_textEdit( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void LinesWidget::setTextEditor( CodeEditorWidget* widget )
{
   m_textEdit = widget;
}

///////////////////////////////////////////////////////////////////////////////

void LinesWidget::paintEvent( QPaintEvent *event )
{
   m_textEdit->paintLines( this, event );
}

///////////////////////////////////////////////////////////////////////////////

void LinesWidget::updateArea( const QRect &rect, int dy )
{
   if ( dy )
   {
      scroll( 0, dy );
   }
   else
   {
      update( 0, rect.y(), width(), rect.height() );
   }
   /*
   if ( rect.contains( viewport()->rect() ) )
   {
      updateLineNumberAreaWidth(0);
   }*/
}

///////////////////////////////////////////////////////////////////////////////

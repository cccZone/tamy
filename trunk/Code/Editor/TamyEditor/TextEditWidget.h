/// @file   TamyEditor/TextEditWidget.h
/// @brief  an enhanced version of the basic text editor widget
#pragma once

#include <QFrame>
#include <QPlainTextEdit>


///////////////////////////////////////////////////////////////////////////////

class QListWidget;
class QPlainTextEdit;
class QLabel;
class TextSyntaxHighlighter;
class QPaintEvent;
class LinesWidget;
class CodeEditorWidget;

///////////////////////////////////////////////////////////////////////////////

class TextEditWidget : public QFrame
{
   Q_OBJECT

private:
   LinesWidget*               m_lineNumbers;
   CodeEditorWidget*          m_editor;
   QLabel*                    m_statusBar;

   TextSyntaxHighlighter*     m_highlighter;

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    */
   TextEditWidget( QWidget* parentWidget ); 
   ~TextEditWidget();

   /**
    * Sets the font the text will be displayed with.
    */
   void setFont( const QFont& font );

   /**
    * Sets a new syntax highlighter.
    *
    * @param highlighter
    */
   void setSyntaxHighlighter( TextSyntaxHighlighter* highlighter );

   /**
    * Replaces the edited document's contents with the specified plain text.
    *
    * @param text
    */
   void setPlainText( const QString& text );

   /**
    * Returns the contents of the edited document as a plain text.
    */
   QString toPlainText() const;

signals:
   /**
    * Emitted when document contents changes.
    */
   void textChanged();

public slots:
   void onTextChanged();
   void onTextCursorMoved();
   void onSelectionChanged();
   void undo();
   void redo();
   void documentScrolled( int scrollValue );

private:
   void highlightCurrentLine();
};

///////////////////////////////////////////////////////////////////////////////

class CodeEditorWidget : public QPlainTextEdit
{
   Q_OBJECT

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    */
   CodeEditorWidget( QWidget* parentWidget );

   /**
    * Paints the lines area.
    *
    * @param linesWidget
    * @param event
    */
   void paintLines( LinesWidget* linesWidget, QPaintEvent* event );
};

///////////////////////////////////////////////////////////////////////////////

class LinesWidget : public QWidget
{
   Q_OBJECT

private:
   CodeEditorWidget*     m_textEdit;

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    */
   LinesWidget( QWidget* parentWidget );

   /**
    * Returns the desired size of the widget
    */
   QSize sizeHint() const { return QSize( 40, 0 ); }

   /**
    * Sets a text editor widget these lines are displayed for.
    *
    * @param widget
    */
   void setTextEditor( CodeEditorWidget* widget );

public slots:
   void updateArea( const QRect &rect, int dy );

protected:
   void paintEvent( QPaintEvent *event );
};

///////////////////////////////////////////////////////////////////////////////

/// @file   TamyEditor/ClosableFrame.h
/// @brief  a frame that can host a widget and wears a close button on, thus can be closed by the user
#pragma once

#include <QFrame>


///////////////////////////////////////////////////////////////////////////////

class QVBoxLayout;

///////////////////////////////////////////////////////////////////////////////

/**
 * A frame that can host a widget and wears a close button on, thus can be closed by the user.
 *
 * The frame WON'T close on its own - when the close button gets clicked, the frame only emits a request,
 * and it's up to us how we want to handle that request.
 */
class ClosableFrame : public QFrame
{
   Q_OBJECT

private:
   QVBoxLayout*   m_mainLayout;
   QWidget*       m_embeddedWidget;

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    */
   ClosableFrame( QWidget* parentWidget );

   /**
    * Embeds a single widget in the frame.
    * If any widget was embedded before, it will be removed from the layout and deleted!!!
    *
    * @param widget
    */
   void setWidget( QWidget* widget );

public slots:
   void onClose();

signals:
   /**
    * A signal emitted when the user clicked the close button.
    *
    * @param widget.
    */
   void onCloseRequest( QWidget* widget );

};

///////////////////////////////////////////////////////////////////////////////

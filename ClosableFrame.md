# Closable frame #

Sometimes you would like to embed a widget inside a layout, but be able to remove it at any time from the editor.

This is where the `ClosableFrame` widget comes in handy.
It creates a frame with a close button on it the user can click.

Once the button is clicked, the frame emits the `onCloseRequest( QWidget* widget )` signal you can connect to a slot that will remove the widget from the layout, like so:

```
// this is the slot we connected the onClosed signal to
void MainFrame::onWidgetClosed( QWidget* closedWidget )
{
   layout()->removeWidget( closedWidget );

   // dispose of the widget
   closedWidget->setParent( NULL );
   closedWidget->deleteLater();
}
```

// and here's a short snippet showing how to setup the frame and embed another widget ( the one we want closed ) in it:

```
void MainFrame::openClosableWidget()
{
   ClosableFrame* closableFrame = new ClosableFrame( this );

   FrameToEmbed* embeddedFrame= new FrameToEmbed( closableFrame );
   connect( frame, SIGNAL( onCloseRequest( QWidget* ) ), this, SLOT( onWidgetClosed( QWidget* ) ) );

   closableFrame->setWidget( embeddedFrame);

   // simply embed the new frame in the main one at the top
   layout()->insertWidget( 1, closableFrame );
}
```


**CAUTION**: the frame WON'T close on its own - when the close button gets clicked, the frame only emits a request, and it's up to us how we want to handle that request.
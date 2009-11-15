#pragma once

/// @file   TamyEditor\TamySceneWidget.h
/// @brief  a widget for displaying tamy scenes

#include <QWidget>
#include <windows.h>
#include "core-AppFlow\UserInputController.h"
#include "core\Point.h"


///////////////////////////////////////////////////////////////////////////////

class IWindowMessagesProcessor;

///////////////////////////////////////////////////////////////////////////////

class TamySceneWidget : public QWidget, public UserInputController
{
   Q_OBJECT

private:
   IWindowMessagesProcessor* m_winMsgProcessor;
   IWindowMessagesProcessor* m_activeWinMsgProcessor;
   HWND m_hWnd;

   unsigned char m_keyBuffer[256];

public:
   /**
    * Constructor.
    *
    * @param parent           parent widget
    * @param f                creation flags
    */
   TamySceneWidget(QWidget* parent, Qt::WindowFlags f);
   ~TamySceneWidget();

   /**
    * The method allows to set an arbitrary window messages processor 
    * for this class instance to use.
    *
    * @param winMsgProcessor  processor that should be informed about 
    *                         window events 
    */
   void setWinMsgProcessor(IWindowMessagesProcessor& winMsgProcessor);

   /**
    * The method gives access to the window handle.
    *
    * @return     window handle
    */
   HWND getWinHandle() const;

protected:
   // -------------------------------------------------------------------------
   // QWidget implementation
   // -------------------------------------------------------------------------
   void moveEvent(QMoveEvent* event);

   void resizeEvent(QResizeEvent* event);

   void keyPressEvent(QKeyEvent* event);

   void keyReleaseEvent(QKeyEvent* event);

   void mousePressEvent(QMouseEvent* event);

   void mouseReleaseEvent(QMouseEvent* event);

   // -------------------------------------------------------------------------
   // UserInputController implementation
   // -------------------------------------------------------------------------
   void onRelativeMouseMovement();

   void onAbsoluteMouseMovement();

   void checkUserInput(unsigned char* keyBuffer, Point& mousePos);

   void setMousePos(const Point& pos);

private:
   unsigned char toDXKey(int qtKeyCode) const;
};

///////////////////////////////////////////////////////////////////////////////

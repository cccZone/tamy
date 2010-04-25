#pragma once

/// @file   TamyEditor\TamySceneWidget.h
/// @brief  a widget for displaying tamy scenes

#include <QWidget>
#include <windows.h>
#include "core.h"
#include "core-AppFlow\UserInputController.h"
#include "core\Point.h"
#include "core-Renderer\Camera.h"
#include "core-MVC\ModelView.h"
#include <d3d9.h>
#include <set>


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class QTimer;
class CTimer;
class Renderer;
class KeysStatusManager;
class Camera;
class Model;

///////////////////////////////////////////////////////////////////////////////

class TamySceneWidget : public QWidget, 
                        public UserInputController,
                        public Component<TamyEditor>
{
   Q_OBJECT

private:
   static IDirect3D9* s_d3d9;

   // window definition
   HWND m_hWnd;

   // input tracking
   unsigned char m_keyBuffer[256];
   KeysStatusManager* m_keysStatusManager;

   // required services
   Renderer* m_renderer;
   Camera* m_camera;
   ResourcesManager& m_resourcesManager;

   Model* m_localModel;

public:
   /**
    * Constructor.
    *
    * @param parent           parent widget
    * @param f                creation flags
    * @param editorRM         resources manager capable of loading editor 
    *                         specific resources
    */
   TamySceneWidget(QWidget* parent, 
      Qt::WindowFlags f, ResourcesManager& editorRM);
   ~TamySceneWidget();

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize(TamyEditor& mgr);

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

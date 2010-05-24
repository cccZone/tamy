#pragma once

/// @file   TamyEditor\TamySceneWidget.h
/// @brief  a widget for displaying tamy scenes

#include <QWidget>
#include <windows.h>
#include "core.h"
#include "core-AppFlow\UserInputController.h"
#include "core\Point.h"
#include "core-Renderer\Camera.h"
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
class DebugRenderer;
class ResourcesManager;
class RendererComponent;

///////////////////////////////////////////////////////////////////////////////

class TamySceneWidget : public QWidget, 
                        public UserInputController,
                        public Component<TamyEditor>
{
   Q_OBJECT

private:
   static IDirect3D9*   s_d3d9;

   // window definition
   HWND                 m_hWnd;

   // input tracking
   unsigned char        m_keyBuffer[256];
   KeysStatusManager*   m_keysStatusManager;

   // required services
   ResourcesManager*    m_resMgr;
   RendererComponent*   m_rendererComponent;
   Renderer*            m_renderer;
   Camera*              m_camera;

   Model*               m_scene;
   DebugRenderer*       m_debugRenderer;

public:
   /**
    * Constructor.
    *
    * @param parent           parent widget
    * @param f                creation flags
    */
   TamySceneWidget( QWidget* parent, Qt::WindowFlags f );
   ~TamySceneWidget();

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );
   void onServiceRegistered( TamyEditor& mgr );

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
   void setupTimeController( TamyEditor& mgr );
   void createRenderer( TamyEditor& mgr );
};

///////////////////////////////////////////////////////////////////////////////

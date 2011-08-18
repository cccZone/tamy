#pragma once

/// @file   TamyEditor\TamySceneWidget.h
/// @brief  a widget for displaying tamy scenes

#include <QWidget>
#include <windows.h>
#include "core.h"
#include "core-AppFlow\UserInputController.h"
#include "core\Point.h"
#include "core-Renderer\Camera.h"
#include "core-MVC\ModelComponent.h"
#include <d3d9.h>
#include <set>


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class Renderer;
class KeysStatusManager;
class Camera;
class Model;
class DebugScene;
class ResourcesManager;
class CompositeRenderingMechanism;
class TimeController;

///////////////////////////////////////////////////////////////////////////////

class TamySceneWidget : public QWidget, 
                        public UserInputController
{
   Q_OBJECT

private:
   static IDirect3D9*                                 s_d3d9;

   // window definition
   HWND                                               m_hWnd;
   std::string                                        m_rendererPipelineName;
   TimeController*                                    m_localTimeController;

   // input tracking
   unsigned char                                      m_keyBuffer[256];
   KeysStatusManager*                                 m_keysStatusManager;

   // required services
   ResourcesManager*                                  m_resMgr;
   Renderer*                                          m_renderer;
   Camera*                                            m_camera;

   Model*                                             m_scene;
   DebugScene*                                        m_debugScene;
   CompositeRenderingMechanism*                       m_renderingMech;

public:
   /**
    * Constructor.
    *
    * @param parent                       parent widget
    * @param f                            creation flags
    * @param rendererPipelineName         path to the rendering pipeline resource
    * @param timeController               time controller that should update the widget
    */
   TamySceneWidget( QWidget* parent, Qt::WindowFlags f, const std::string& rendererPipelineName, TimeController& timeController );
   ~TamySceneWidget();

   /**
    * Allows to set up a rendered scene.
    *
    * @param scene
    */
   void setScene( Model& scene );

   // -------------------------------------------------------------------------
   // Accessors
   // -------------------------------------------------------------------------
   inline Renderer& getRenderer() const { return *m_renderer; }

   inline Camera& getCamera() const { return *m_camera; }

   inline KeysStatusManager& getKeysStatusManager() const { return *m_keysStatusManager; }

   inline DebugScene& getDebugScene() const { return *m_debugScene; }

   inline CompositeRenderingMechanism& getRenderingMech() const { return *m_renderingMech; }

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
   void setupTimeController();
   void initialize();
   void deinitialize();
};

///////////////////////////////////////////////////////////////////////////////

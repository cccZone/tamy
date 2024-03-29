/// @file   TamyEditor\TamySceneWidget.h
/// @brief  a widget for displaying scenes
#pragma once

#include <QWidget>
#include <windows.h>
#include "core.h"
#include "core-AppFlow\UserInputController.h"
#include "core\Point.h"
#include "core-Renderer\Camera.h"
#include "core-MVC\ModelComponent.h"
#include "SelectionManagerListener.h"
#include "Gizmo.h"
#include <d3d9.h>
#include <set>


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class Renderer;
class Camera;
class Model;
class ResourcesManager;
class CompositeRenderingMechanism;
class TimeController;
class SelectionRenderingPass;
class SceneRendererInputController;
class TimeControllerTrack;
class QueryRenderingPass;
class SceneQuery;
class EditorDebugRenderer;
class DebugEntitiesManager;

///////////////////////////////////////////////////////////////////////////////

class TamySceneWidget : public QWidget, public UserInputController, public SelectionManagerListener
{
   Q_OBJECT

private:
   static IDirect3D9*                                 s_d3d9;

   // window definition
   HWND                                               m_hWnd;
   FilePath                                           m_rendererPipelineName;
   TimeController*                                    m_localTimeController;

   // input tracking
   unsigned char                                      m_keyBuffer[256];
   TimeControllerTrack*                               m_inputHandlerTrack;
   TimeControllerTrack*                               m_gizmoTrack;

   // required services
   ResourcesManager*                                  m_resMgr;
   Renderer*                                          m_renderer;
   Camera*                                            m_camera;

   Model*                                             m_scene;
   CompositeRenderingMechanism*                       m_renderingMech;
   SelectionRenderingPass*                            m_selectionRenderer;
   QueryRenderingPass*                                m_queryRenderer;

   // debug renderer
   EditorDebugRenderer*                               m_debugRenderer;
   DebugEntitiesManager*                              m_debugEntitiesManager;

   // a gizmo that will be displayed when a scene object that can be manipulated gets selected
   Gizmo::Mode                                        m_gizmoMode;
   Gizmo*                                             m_gizmo;

public:
   /**
    * Constructor.
    *
    * @param parent                       parent widget
    * @param f                            creation flags
    * @param rendererPipelineName         path to the rendering pipeline resource
    * @param timeController               time controller that should update the widget
    */
   TamySceneWidget( QWidget* parent, Qt::WindowFlags f, const FilePath& rendererPipelineName, TimeController& timeController );
   ~TamySceneWidget();

   /**
    * Allows to set up a rendered scene.
    *
    * @param scene
    */
   void setScene( Model& scene );

   /**
    * Stops rendering the scene that was previously set.
    */
   void clearScene();

   /**
    * Sets a new input controller.
    *
    * @param controller
    */
   void setInputController( SceneRendererInputController* controller );

   /**
    * Issues a scene entity selection query.
    *
    * @param query
    */
   void queryScene( SceneQuery& query ) const;

   /**
    * The method translates the screen space coordinates
    * (i.e. ones your mouse moves in) to viewport space coordinates
    */
   void localToViewport( const Point& inScreenPt, Vector& outViewportPt ) const;

   /**
    * Changes the pipeline the widget uses to render stuff.
    *
    * @param pipeline
    */
   void setRenderingPipeline( const FilePath& pipeline );

   // -------------------------------------------------------------------------
   // Objects manipulation gizmo management
   // -------------------------------------------------------------------------

   /**
    * Sets the objects manipulation gizmo in the translation mode.
    */
   void setGizmoTranslationMode();

   /**
    * Sets the objects manipulation gizmo in the rotation mode.
    */
   void setGizmoRotationMode();

   /**
    * Returns currently set gizmo manipulation mode.
    */
   inline Gizmo::Mode getGizmoMode() const { return m_gizmoMode; }

   // -------------------------------------------------------------------------
   // Accessors
   // -------------------------------------------------------------------------
   inline Renderer& getRenderer() const { return *m_renderer; }

   inline Camera& getCamera() const { return *m_camera; }

   inline CompositeRenderingMechanism& getRenderingMech() const { return *m_renderingMech; }

   /**
    * Gives access to the scene queries renderer, allowing to toggle certain debugging features.
    */
   inline QueryRenderingPass& getQueryRenderer() const { return *m_queryRenderer; }

   /**
    * Gives access to the debug entities manager that allows to toggle certain debugging features.
    */
   inline DebugEntitiesManager& getDebugEntitiesManager() const { return *m_debugEntitiesManager; }
   
   /**
    * Returns filepath to the currently used rendering pipeline resource.
    */
   const FilePath& getRenderingPipeline() const { return m_rendererPipelineName; }

   // -------------------------------------------------------------------------
   // SelectionManagerListener implementation
   // -------------------------------------------------------------------------
   void onEntitySelected( Entity& entity );
   void onEntityDeselected( Entity& entity );

protected:
   // -------------------------------------------------------------------------
   // QWidget implementation
   // -------------------------------------------------------------------------
   void moveEvent( QMoveEvent* event );
   void resizeEvent( QResizeEvent* event );
   void keyPressEvent( QKeyEvent* event );
   void keyReleaseEvent( QKeyEvent* event );
   void mousePressEvent( QMouseEvent* event );
   void mouseReleaseEvent( QMouseEvent* event );
   void showEvent( QShowEvent* event );
   void hideEvent( QHideEvent* event );
   void focusOutEvent( QFocusEvent* event );

   // -------------------------------------------------------------------------
   // UserInputController implementation
   // -------------------------------------------------------------------------
   void onRelativeMouseMovement();
   void onAbsoluteMouseMovement();
   void checkUserInput(unsigned char* keyBuffer, Point& mousePos);
   void setMousePos(const Point& pos);

private:
   unsigned char toDXKey(int qtKeyCode) const;
   void initialize();
   void deinitialize();
};

///////////////////////////////////////////////////////////////////////////////

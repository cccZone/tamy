#pragma once

/// @file   TamyEditor\tamyeditor.h
/// @brief  editor's main window class

#include <QtGui/QMainWindow>
#include "ui_tamyeditor.h"
#include "core.h"

///////////////////////////////////////////////////////////////////////////////

class QApplication;
class QTimer;
class Renderer;
class Model;
class UserInputController;
class UnconstrainedMotionController;
class CTimer;
class SpatiallyQueryable;
class EntitiesStorageView;
class KeysStatusManager;
struct Point;
class PropertiesView;

namespace RendererView
{
   class RendererView;
}

///////////////////////////////////////////////////////////////////////////////

/**
* This is the editor's main window class.
*/
class TamyEditor : public QMainWindow
{
   Q_OBJECT

private:
   QApplication& m_app;
   Ui::TamyEditorClass ui;
   CTimer* m_mainTime;
   CTimer* m_inputTime;
   QTimer* m_mainTimeSlot;
   QTimer* m_inputTimeSlot;
   Renderer* m_renderer;

   Model* m_scene;
   RendererView::RendererView* m_renderView;

   UserInputController* m_renderWinUiController;
   KeysStatusManager* m_keysStatusManager;
   UnconstrainedMotionController* m_cameraController;

   SpatialStorage<SpatiallyQueryable>* m_entitiesQueryStorage;

   PropertiesView* m_selectionManager;

public:
   /**
    * Constructor.
    *
    * @param app        qt application running the show
    * @param parent     parent widget
    * @param flags      widget creation flags
    */
   TamyEditor(QApplication& app, QWidget *parent = 0, Qt::WFlags flags = 0);
   ~TamyEditor();

public slots:
   void updateMain();
   void updateInput();

   void loadScene();
   void saveScene();
   void importScene();

private:
   void performSceneQuery(const Point& mousePos, Array<SpatiallyQueryable*>& nodes);
};

///////////////////////////////////////////////////////////////////////////////

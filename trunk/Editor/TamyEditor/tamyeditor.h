#pragma once

/// @file   TamyEditor\tamyeditor.h
/// @brief  editor's main window class


#include <QtGui/QMainWindow>
#include "ui_tamyeditor.h"


///////////////////////////////////////////////////////////////////////////////

class QApplication;
class QTimer;
class Renderer;
class Model;

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
   QTimer* m_timer;
   Renderer* m_renderer;

   Model* m_scene;
   RendererView::RendererView* m_renderView;

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
    /**
     * Internal timer will call this method periodically.
     */
    void update();

    void loadScene();
    void saveScene();
    void importScene();
};

///////////////////////////////////////////////////////////////////////////////

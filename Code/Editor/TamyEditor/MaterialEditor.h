/// @file   TamyEditor/MaterialEditor.h
/// @brief  material editor's main class

#pragma once

#include <QtGui/QMainWindow>
#include "ui_materialeditor.h"
#include "ResourceEditor.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialSyntaxHighlighter;
class PixelShader;
class ResourcesManager;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Material editor's main class.
 */
class MaterialEditor : public QMainWindow, public ResourceEditor
{
   Q_OBJECT

private:
   Ui::MaterialEditorWindow         m_ui;

   PixelShader&                     m_shader;
   MaterialSyntaxHighlighter*       m_highlighter;

   ResourcesManager*                m_resourceMgr;
   Renderer*                        m_renderer;
   bool                             m_docModified;

public:
   /**
    * Constructor.
    *
    * @param shader     edited shader
    */
   MaterialEditor( PixelShader& shader );
   ~MaterialEditor();

   // -------------------------------------------------------------------------
   // ResourceEditor implementation
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );

public slots:
   void loadFromFile();
   void save();
   void compile();
   void onScriptModified();

protected:
   void closeEvent( QCloseEvent *event );

private:
   void synchronize();
};

///////////////////////////////////////////////////////////////////////////////

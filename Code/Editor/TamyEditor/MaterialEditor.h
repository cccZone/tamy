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
class MaterialEditor : public ResourceEditor
{
   Q_OBJECT

private:
   Ui::MaterialEditorWindow         m_ui;

   PixelShader&                     m_shader;
   MaterialSyntaxHighlighter*       m_highlighter;

   ResourcesManager*                m_resourceMgr;
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
   void onInitialize();
   void onDeinitialize( bool saveProgress );

public slots:
   void save();
   bool compile();
   void onScriptModified();
   void onTextCursorMoved();
   void onParamChange();

private:
   void synchronize();
};

///////////////////////////////////////////////////////////////////////////////

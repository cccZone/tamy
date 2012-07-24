/// @file   TamyEditor/VertexShaderEditor.h
/// @brief  vertex shader editor's main class

#pragma once

#include <QtGui/QMainWindow>
#include "ui_vertexshadereditor.h"
#include "ResourceEditor.h"


///////////////////////////////////////////////////////////////////////////////

class ShaderSyntaxHighlighter;
class VertexShader;
class ResourcesManager;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Vertex shader editor's main class.
 */
class VertexShaderEditor : public ResourceEditor
{
   Q_OBJECT

private:
   enum VSETabs
   {
      VSE_TAB_PROPERTIES,
      VSE_TAB_OUTPUT
   };

private:
   Ui::VertexShaderEditorWindow     m_ui;

   VertexShader&                    m_shader;
   ShaderSyntaxHighlighter*         m_highlighter;

   ResourcesManager*                m_resourceMgr;
   bool                             m_docModified;

public:
   /**
    * Constructor.
    *
    * @param shader     edited shader
    */
   VertexShaderEditor( VertexShader& shader );
   ~VertexShaderEditor();

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
   void onVertexIdChange( const QString& newId );
   void importFrom();
   void exportTo();

private:
   void synchronize();
};

///////////////////////////////////////////////////////////////////////////////

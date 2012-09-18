/// @file   TamyEditor/FragmentShaderEditor.h
/// @brief  fragment shader ( shader functions libs ) editor's main class

#pragma once

#include <QtGui/QMainWindow>
#include "ui_fragmentshadereditor.h"
#include "ResourceEditor.h"


///////////////////////////////////////////////////////////////////////////////

class FragmentShader;
class ResourcesManager;
class Renderer;
class TextEditWidget;

///////////////////////////////////////////////////////////////////////////////

/**
 * Fragment shader ( vertex shader, shader functions libs ) editor's main class.
 */
class FragmentShaderEditor : public ResourceEditor
{
   Q_OBJECT

private:
   Ui::FragmentShaderEditorWindow   m_ui;

   FragmentShader&                  m_shader;

   TextEditWidget*                  m_scriptEditor;
   ResourcesManager*                m_resourceMgr;
   bool                             m_docModified;

public:
   /**
    * Constructor.
    *
    * @param shader     edited shader
    */
   FragmentShaderEditor( FragmentShader& shader );
   ~FragmentShaderEditor();

   // -------------------------------------------------------------------------
   // ResourceEditor implementation
   // -------------------------------------------------------------------------
   void onInitialize();
   void onDeinitialize( bool saveProgress );

public slots:
   void save();
   void onScriptModified();
   void importFrom();
   void exportTo();

private:
   void synchronize();
};

///////////////////////////////////////////////////////////////////////////////

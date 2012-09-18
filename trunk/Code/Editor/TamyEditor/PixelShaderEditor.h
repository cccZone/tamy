/// @file   TamyEditor/PixelShaderEditor.h
/// @brief  pixel shader editor's main class

#pragma once

#include <QtGui/QMainWindow>
#include "ui_pixelshadereditor.h"
#include "ResourceEditor.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class ShaderSyntaxHighlighter;
class PixelShader;
class ResourcesManager;
class Renderer;
class QTabWidget;
class QComboBox;
class ColorFrame;
class HexEditor;
class TextEditWidget;

///////////////////////////////////////////////////////////////////////////////

/**
 * Pixel shader editor's main class.
 */
class PixelShaderEditor : public ResourceEditor
{
   Q_OBJECT

private:
   struct TextureStageTabDef
   {
      QComboBox*        m_uAddressMode;
      QComboBox*        m_vAddressMode;
      QComboBox*        m_wAddressMode;
      QComboBox*        m_minFilter;
      QComboBox*        m_magFilter;
      QComboBox*        m_mipFilter;
      TextureStageTabDef()
         : m_uAddressMode( NULL )
         , m_vAddressMode( NULL )
         , m_wAddressMode( NULL )
         , m_minFilter( NULL )
         , m_magFilter( NULL )
         , m_mipFilter( NULL )
      {
      }
   };

   enum PSETabs
   {
      PSE_TAB_RENDERING,
      PSE_TAB_ALPHA_BLENDING,
      PSE_TAB_STENCIL,
      PSE_TAB_TEXTURE_STAGES,
      PSE_TAB_OUTPUT
   };

private:
   Ui::PixelShaderEditorWindow         m_ui;

   QStringList                         m_addressModes;
   QStringList                         m_filteringModes;
   QStringList                         m_comparisonFunctions;
   QStringList                         m_blendFunctions;

   PixelShader&                        m_shader;
   TextEditWidget*                     m_scriptEditor;
   ShaderSyntaxHighlighter*            m_highlighter;
   QTabWidget*                         m_textureStagesTabs;
   std::vector< TextureStageTabDef >   m_textureStagesTabWidgets;
   ColorFrame*                         m_alphaTestRefColor;
   HexEditor*                          m_stencilRef;
   HexEditor*                          m_stencilMask;
   HexEditor*                          m_stencilWriteMask;

   ResourcesManager*                   m_resourceMgr;
   bool                                m_docModified;

public:
   /**
    * Constructor.
    *
    * @param shader     edited shader
    */
   PixelShaderEditor( PixelShader& shader );
   ~PixelShaderEditor();

   // -------------------------------------------------------------------------
   // ResourceEditor implementation
   // -------------------------------------------------------------------------
   void onInitialize();
   void onDeinitialize( bool saveProgress );

public slots:
   void save();
   bool compile();
   void onScriptModified();
   void onParamChange();
   void onTextureStageChange();
   void importFrom();
   void exportTo();

private:
   void synchronize();
   void initializeTextureStagesTab();
   void refreshRenderingParamsUI();
};

///////////////////////////////////////////////////////////////////////////////

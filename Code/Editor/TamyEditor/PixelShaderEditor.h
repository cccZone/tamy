/// @file   TamyEditor/PixelShaderEditor.h
/// @brief  pixel shader editor's main class

#pragma once

#include <QtGui/QMainWindow>
#include "ui_pixelshadereditor.h"
#include "ResourceEditor.h"
#include <vector>
#include <string>
#include "core-Renderer/PixelShader.h"
#include <QIcon>


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
class QPushButton;

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
      QPushButton*      m_linkAddressingModes;
      bool              m_addressingModesLinked;
      QPushButton*      m_linkFilters;
      bool              m_filteringModesLinked;
      QComboBox*        m_uAddressMode;
      QComboBox*        m_vAddressMode;
      QComboBox*        m_wAddressMode;
      QComboBox*        m_minFilter;
      QComboBox*        m_magFilter;
      QComboBox*        m_mipFilter;
      TextureStageTabDef()
         : m_linkAddressingModes( NULL )
         , m_addressingModesLinked( false )
         , m_linkFilters( NULL )
         , m_filteringModesLinked( false )
         , m_uAddressMode( NULL )
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

   // A helper structure keeping all texture stage entries that were used during
   // editor's lifetime, so that if a stage is brought back to life, we can restore its
   // settings
   struct TextureStageEntry
   {
      std::string          m_textureStageName;
      TextureStageParams   m_params;

      TextureStageEntry( const std::string& name, const TextureStageParams& params )
         : m_textureStageName( name )
         , m_params( params )
      {}
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

   QIcon                               m_linkActiveIcon;
   QIcon                               m_linkInactiveIcon;

   ResourcesManager*                   m_resourceMgr;
   bool                                m_docModified;

   std::vector< TextureStageEntry >    m_textureStageSettingsDB;

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
   void onToggleAddressingModesLink( bool clicked );
   void onToggleFiltersLink( bool clicked );

private:
   void synchronize();
   void initializeTextureStagesTab();
   void refreshRenderingParamsUI();
   void updateAddressingModesLinkState( int textureStage );
   void updateFiltersLinkState( int textureStage );

   // -------------------------------------------------------------------------
   // Texture stage settings DB
   // -------------------------------------------------------------------------
   void addTextureStageToDB( const std::string& name, const TextureStageParams& params );
   void restoreTextureStageFromDB( const std::string& name, TextureStageParams& outParams );
};

///////////////////////////////////////////////////////////////////////////////

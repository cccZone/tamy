#include "PixelShaderEditor.h"
#include "ShaderSyntaxHighlighter.h"
#include "tamyeditor.h"
#include "core.h"
#include "core-Renderer.h"
#include "ColorFrame.h"
#include "HexEditor.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox.h>
#include <QTextCursor>
#include <QTabWidget>
#include <QComboBox>
#include <QTextCursor>
#include "TextEditWidget.h"


///////////////////////////////////////////////////////////////////////////////

PixelShaderEditor::PixelShaderEditor( PixelShader& shader )
   : m_shader( shader )
   , m_scriptEditor( NULL )
   , m_highlighter( NULL )
   , m_resourceMgr( NULL )
   , m_textureStagesTabs( NULL )
{
   m_addressModes << "Wrap" << "Mirror" << "Clamp" << "Border" << "Mirror once";
   m_filteringModes << "None" << "Point" << "Linear" << "Anisotropic" << "Pyramidal quad" << "Gaussian quad";
}

///////////////////////////////////////////////////////////////////////////////

PixelShaderEditor::~PixelShaderEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void PixelShaderEditor::onInitialize()
{
   m_resourceMgr = &ResourcesManager::getInstance();

   ASSERT( m_resourceMgr != NULL );

   // setup ui
   m_ui.setupUi( this );

   // set the editor up
   m_scriptEditor = new TextEditWidget( m_ui.editorFrame );
   m_ui.editorFrame->layout()->addWidget( m_scriptEditor );
   QFont font( "Courier", 12 );
   m_scriptEditor->setFont( font );
   m_scriptEditor->setSyntaxHighlighter( new ShaderSyntaxHighlighter() );
   m_scriptEditor->setPlainText( m_shader.getScript().c_str() );
   connect( m_scriptEditor, SIGNAL( textChanged() ), this, SLOT( onScriptModified() ) );
   m_docModified = false;

   // set the properties
   {
      const PixelShaderParams& params = m_shader.getParams();

      // geometry group box
      {
         m_ui.cullingMode->setCurrentIndex( params.m_cullingMode - 1 );
         connect( m_ui.cullingMode, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );
      }

      // depth group box
      {
         m_ui.zBufferEnabled->setChecked( params.m_useZBuffer );
         connect( m_ui.zBufferEnabled, SIGNAL( toggled( bool ) ), this, SLOT( onParamChange() ) );

         m_ui.zBufferWriteEnabled->setChecked( params.m_writeToZBuffer );
         connect( m_ui.zBufferWriteEnabled, SIGNAL( toggled( bool ) ), this, SLOT( onParamChange() ) );

         m_ui.depthTestFuncCombo->setCurrentIndex( params.m_depthTestFunc - 1 );
         connect( m_ui.depthTestFuncCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );
      }

      // alpha test group box
      {
         m_ui.alphaTest->setChecked( params.m_useAlphaTest );
         connect( m_ui.alphaTest, SIGNAL( toggled( bool ) ), this, SLOT( onParamChange() ) );

         m_ui.alphaTestFuncCombo->setCurrentIndex( params.m_alphaTestFunc - 1 );
         connect( m_ui.alphaTestFuncCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

         // create the reference color picker
         {
            QLayout* layout = m_ui.alphaTestRefColor->layout();

            Color color;
            color.setFromLong( params.m_alphaTestRefVal );
            m_alphaTestRefColor = new ColorFrame( m_ui.alphaTestRefColor, color );
            layout->addWidget( m_alphaTestRefColor );
            connect( m_alphaTestRefColor, SIGNAL( changed( const QColor& ) ), this, SLOT( onParamChange() ) );
         }
      }

      // blending group box
      {
         m_ui.useAlphaBlend->setChecked( params.m_useBlending );
         connect( m_ui.useAlphaBlend, SIGNAL( toggled( bool ) ), this, SLOT( onParamChange() ) );

         m_ui.blendSrcFuncCombo->setCurrentIndex( params.m_blendSourceFunc - 1 );
         connect( m_ui.blendSrcFuncCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

         m_ui.blendDestFuncCombo->setCurrentIndex( params.m_blendDestFunc - 1 );
         connect( m_ui.blendDestFuncCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );
      }

      // separate alpha blending group box
      {
         m_ui.useSeparatedAlphaBlending->setChecked( params.m_useSeparateAlphaBlend );
         connect( m_ui.useSeparatedAlphaBlending, SIGNAL( toggled( bool ) ), this, SLOT( onParamChange() ) );

         m_ui.alphaBlendSrcFuncCombo->setCurrentIndex( params.m_alphaBlendSourceFunc - 1 );
         connect( m_ui.alphaBlendSrcFuncCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

         m_ui.alphaBlendDestFuncCombo->setCurrentIndex( params.m_alphaBlendDestFunc - 1 );
         connect( m_ui.alphaBlendDestFuncCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );
      }

      // stencil operations group box
      {
         m_ui.useStencil->setChecked( params.m_stencilEnable );
         connect( m_ui.useStencil, SIGNAL( toggled( bool ) ), this, SLOT( onParamChange() ) );

         m_ui.stencilFailCombo->setCurrentIndex( params.m_stencilFail - 1 );
         connect( m_ui.stencilFailCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

         m_ui.stencilZFailCombo->setCurrentIndex( params.m_stencilZFail - 1 );
         connect( m_ui.stencilZFailCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

         m_ui.stencilPassCombo->setCurrentIndex( params.m_stencilPass - 1 );
         connect( m_ui.stencilPassCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

         m_ui.stencilFuncCombo->setCurrentIndex( params.m_stencilFunc - 1 );
         connect( m_ui.stencilFuncCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

         m_stencilRef = new HexEditor( m_ui.stencilRefFrame, params.m_stencilRef );
         m_ui.stencilRefFrame->layout()->addWidget( m_stencilRef );

         m_stencilMask = new HexEditor( m_ui.stencilMaskFrame, params.m_stencilMask );
         m_ui.stencilMaskFrame->layout()->addWidget( m_stencilMask );

         m_stencilWriteMask = new HexEditor( m_ui.stencilWriteMaskFrame, params.m_stencilWriteMask );
         m_ui.stencilWriteMaskFrame->layout()->addWidget( m_stencilWriteMask );
      }

      refreshRenderingParamsUI();
   }

   // set texture stages tabs
   {
      initializeTextureStagesTab();
   }

   // set the toolbar
   QString iconsDir = m_resourceMgr->getFilesystem().getShortcut( "editorIcons" ).c_str();
   
   QToolBar* toolbar = new QToolBar( m_ui.toolbarFrame );
   m_ui.toolbarFrame->layout()->addWidget( toolbar );

   // file management
   {
      QAction* actionSave = new QAction( QIcon( iconsDir + tr( "/saveFile.png" ) ), tr( "Save" ), toolbar );
      actionSave->setShortcut( QKeySequence( tr( "Ctrl+S" ) ) );
      toolbar->addAction( actionSave );
      connect( actionSave, SIGNAL( triggered() ), this, SLOT( save() ) );

      QAction* actionImportFrom = new QAction( QIcon( iconsDir + tr( "/import.png" ) ), tr( "Import" ), toolbar );
      toolbar->addAction( actionImportFrom );
      connect( actionImportFrom, SIGNAL( triggered() ), this, SLOT( importFrom() ) );

      QAction* actionExportTo = new QAction( QIcon( iconsDir + tr( "/export.png" ) ), tr( "Export" ), toolbar );
      toolbar->addAction( actionExportTo );
      connect( actionExportTo, SIGNAL( triggered() ), this, SLOT( exportTo() ) );

      toolbar->addSeparator();
   }

   // script edition commands
   {
      QAction* actionUndo = new QAction( QIcon( iconsDir + tr( "/undo.png" ) ), tr( "Undo" ), toolbar );
      actionUndo->setShortcut( QKeySequence( tr( "Ctrl+Z" ) ) );
      toolbar->addAction( actionUndo );
      connect( actionUndo, SIGNAL( triggered() ), m_scriptEditor, SLOT( undo() ) );

      QAction* actionRedo = new QAction( QIcon( iconsDir + tr( "/redo.png" ) ), tr( "Redo" ), toolbar );
      actionRedo->setShortcut( QKeySequence( tr( "Ctrl+Y" ) ) );
      toolbar->addAction( actionRedo );
      connect( actionRedo, SIGNAL( triggered() ), m_scriptEditor, SLOT( redo() ) );

      toolbar->addSeparator();
   }

   // building commands
   {
      QAction* actionCompile = new QAction( QIcon( iconsDir + tr( "/play.png" ) ), tr( "Compile" ), toolbar );
      actionCompile->setShortcut( QKeySequence( tr( "F5" ) ) );
      toolbar->addAction( actionCompile );
      connect( actionCompile, SIGNAL( triggered() ), this, SLOT( compile() ) );
   }

   show();
}

///////////////////////////////////////////////////////////////////////////////

void PixelShaderEditor::onDeinitialize( bool saveProgress )
{
   if ( saveProgress && m_docModified )
   {
      int choice = QMessageBox::question( this, tr("Save"), tr("Would you like to save your changes?"), QMessageBox::Yes | QMessageBox::No );
      if ( choice == QMessageBox::Yes )
      {
         save();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void PixelShaderEditor::save()
{
   if ( compile() )
   {
      synchronize();
      m_shader.saveResource();
      m_docModified = false;
   }
}

///////////////////////////////////////////////////////////////////////////////

bool PixelShaderEditor::compile()
{
   m_ui.compilationOutput->clear();

   std::string shaderContents = m_scriptEditor->toPlainText().toStdString();
   if ( shaderContents.empty() )
   {
      m_ui.compilationOutput->setText( "No code to compile" );
      return true;
   }

   ShaderCompiler compiler;
   bool status = compiler.compilePixelShader( shaderContents, m_shader.getEntryFunctionName().c_str() );

   // show the compile output tab
   m_ui.tabWidget->setCurrentIndex( PSE_TAB_OUTPUT );

   // output the status
   if ( status )
   {
      // check the required vertex shader technique
      std::string vsTechniqueName = compiler.parseRequiredVertexShaderTechnique( shaderContents );

      m_ui.compilationOutput->setText( tr( "Compilation successful\nRequred vertex shader technique: " ) + ( vsTechniqueName.empty() ? tr( "<<default>>" ) : vsTechniqueName.c_str() ) );
      return true;
   }
   else
   {
      m_ui.compilationOutput->setText( compiler.getLastError().c_str() );
      return false;
   }
}

///////////////////////////////////////////////////////////////////////////////

void PixelShaderEditor::onParamChange()
{
   PixelShaderParams& params = m_shader.changeParams();

   // copy the values
   params.m_cullingMode = (CullingMode)( m_ui.cullingMode->currentIndex() + 1 );

   params.m_useZBuffer = m_ui.zBufferEnabled->isChecked();
   params.m_writeToZBuffer = m_ui.zBufferWriteEnabled->isChecked();
   params.m_depthTestFunc = (ComparisonFunc)( m_ui.depthTestFuncCombo->currentIndex() + 1 );

   params.m_useAlphaTest = m_ui.alphaTest->isChecked();
   params.m_alphaTestFunc = (ComparisonFunc)( m_ui.alphaTestFuncCombo->currentIndex() + 1 );
   params.m_alphaTestRefVal = (ulong)m_alphaTestRefColor->getEngineColor();

   params.m_useBlending = m_ui.useAlphaBlend->isChecked();
   params.m_blendSourceFunc = (BlendFunc)( m_ui.blendSrcFuncCombo->currentIndex() + 1 );
   params.m_blendDestFunc = (BlendFunc)( m_ui.blendDestFuncCombo->currentIndex() + 1 );

   params.m_useSeparateAlphaBlend = m_ui.useSeparatedAlphaBlending->isChecked();
   params.m_alphaBlendSourceFunc = (BlendFunc)( m_ui.alphaBlendSrcFuncCombo->currentIndex() + 1 );
   params.m_alphaBlendDestFunc = (BlendFunc)( m_ui.alphaBlendDestFuncCombo->currentIndex() + 1 );

   params.m_stencilEnable = m_ui.useStencil->isChecked();
   params.m_stencilFail = (StencilOp)( m_ui.stencilFailCombo->currentIndex() + 1 );
   params.m_stencilZFail = (StencilOp)( m_ui.stencilZFailCombo->currentIndex() + 1 );
   params.m_stencilPass = (StencilOp)( m_ui.stencilPassCombo->currentIndex() + 1 );
   params.m_stencilFunc = (ComparisonFunc)( m_ui.stencilFuncCombo->currentIndex() + 1 );
   params.m_stencilRef = m_stencilRef->getValue();
   params.m_stencilMask = m_stencilMask->getValue();
   params.m_stencilWriteMask = m_stencilWriteMask->getValue();

   refreshRenderingParamsUI();
}

///////////////////////////////////////////////////////////////////////////////

void PixelShaderEditor::refreshRenderingParamsUI()
{
   const PixelShaderParams& params = m_shader.getParams();

   // enable/disable certain fields depending on whether the group they depend on was enabled or disabled
   m_ui.depthTestFuncCombo->setEnabled( params.m_useZBuffer );

   m_ui.alphaTestFuncCombo->setEnabled( params.m_useAlphaTest );
   m_alphaTestRefColor->setEnabled( params.m_useAlphaTest );

   m_ui.blendSrcFuncCombo->setEnabled( params.m_useBlending );
   m_ui.blendDestFuncCombo->setEnabled( params.m_useBlending );

   m_ui.useSeparatedAlphaBlending->setEnabled( params.m_useBlending );
   m_ui.alphaBlendSrcFuncCombo->setEnabled( params.m_useBlending && params.m_useSeparateAlphaBlend );
   m_ui.alphaBlendDestFuncCombo->setEnabled( params.m_useBlending && params.m_useSeparateAlphaBlend );

   m_ui.stencilFailCombo->setEnabled( params.m_stencilEnable );
   m_ui.stencilZFailCombo->setEnabled( params.m_stencilEnable );
   m_ui.stencilPassCombo->setEnabled( params.m_stencilEnable );
   m_ui.stencilFuncCombo->setEnabled( params.m_stencilEnable );
   m_stencilRef->setEnabled( params.m_stencilEnable );
   m_stencilMask->setEnabled( params.m_stencilEnable );
   m_stencilWriteMask->setEnabled( params.m_stencilEnable );
}

///////////////////////////////////////////////////////////////////////////////

void PixelShaderEditor::initializeTextureStagesTab()
{
   int activeTabIdx = 0;

   QLayout* layout = m_ui.textureStagesOptionsTab->layout();
   if ( m_textureStagesTabs != NULL )
   {
      activeTabIdx = m_textureStagesTabs->currentIndex();

      layout->removeWidget( m_textureStagesTabs );
      delete m_textureStagesTabs;
   }

   m_textureStagesTabs = new QTabWidget( m_ui.textureStagesOptionsTab );
   layout->addWidget( m_textureStagesTabs );

   m_textureStagesTabWidgets.clear();

   // add new tabs
   unsigned int count = m_shader.getTextureStagesCount();
   for ( unsigned int i = 0; i < count; ++i )
   {
      QFrame* frame = new QFrame( m_textureStagesTabs );
      QFormLayout* layout = new QFormLayout( frame );
      frame->setLayout( layout );

      m_textureStagesTabs->addTab( frame, m_shader.getTextureStageName( i ).c_str() );

      // add the widgets
      m_textureStagesTabWidgets.push_back( TextureStageTabDef() );
      TextureStageTabDef& tabDef = m_textureStagesTabWidgets.back();

      const TextureStageParams& stageSettings = m_shader.getTextureStage( i );

      tabDef.m_uAddressMode = new QComboBox( frame );
      tabDef.m_uAddressMode->addItems( m_addressModes );
      tabDef.m_uAddressMode->setCurrentIndex( stageSettings.m_addressU - 1 );
      connect( tabDef.m_uAddressMode, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onTextureStageChange() ) );

      tabDef.m_vAddressMode = new QComboBox( frame );
      tabDef.m_vAddressMode->addItems( m_addressModes );
      tabDef.m_vAddressMode->setCurrentIndex( stageSettings.m_addressV - 1 );
      connect( tabDef.m_vAddressMode, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onTextureStageChange() ) );

      tabDef.m_wAddressMode = new QComboBox( frame );
      tabDef.m_wAddressMode->addItems( m_addressModes );
      tabDef.m_wAddressMode->setCurrentIndex( stageSettings.m_addressW - 1 );
      connect( tabDef.m_wAddressMode, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onTextureStageChange() ) );

      tabDef.m_minFilter = new QComboBox( frame );
      tabDef.m_minFilter->addItems( m_filteringModes );
      tabDef.m_minFilter->setCurrentIndex( stageSettings.m_minFilter );
      connect( tabDef.m_minFilter, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onTextureStageChange() ) );

      tabDef.m_magFilter = new QComboBox( frame );
      tabDef.m_magFilter->addItems( m_filteringModes );
      tabDef.m_magFilter->setCurrentIndex( stageSettings.m_magFilter );
      connect( tabDef.m_magFilter, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onTextureStageChange() ) );

      tabDef.m_mipFilter = new QComboBox( frame );
      tabDef.m_mipFilter->addItems( m_filteringModes );
      tabDef.m_mipFilter->setCurrentIndex( stageSettings.m_mipFilter );
      connect( tabDef.m_mipFilter, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onTextureStageChange() ) );

      layout->addRow( "U address mode:", tabDef.m_uAddressMode );
      layout->addRow( "V address mode:", tabDef.m_vAddressMode );
      layout->addRow( "W address mode:", tabDef.m_wAddressMode );
      layout->addRow( "Min filter:", tabDef.m_minFilter );
      layout->addRow( "Mag filter:", tabDef.m_magFilter );
      layout->addRow( "Mip filter:", tabDef.m_mipFilter );
   }

   m_textureStagesTabs->setCurrentIndex( activeTabIdx );
}

///////////////////////////////////////////////////////////////////////////////

void PixelShaderEditor::onTextureStageChange()
{
   // refresh tabs new tabs
   unsigned int count = m_shader.getTextureStagesCount();
   for ( unsigned int i = 0; i < count; ++i )
   {
      TextureStageTabDef& tabDef = m_textureStagesTabWidgets[i];
      TextureStageParams& stageSettings = m_shader.changeTextureStage( i );

      stageSettings.m_addressU = (TextureAddressingMode)( tabDef.m_uAddressMode->currentIndex() + 1 );
      stageSettings.m_addressV = (TextureAddressingMode)( tabDef.m_vAddressMode->currentIndex() + 1 );
      stageSettings.m_addressW = (TextureAddressingMode)( tabDef.m_wAddressMode->currentIndex() + 1 );
      stageSettings.m_minFilter = (TextureFilteringMode)( tabDef.m_minFilter->currentIndex() );
      stageSettings.m_magFilter = (TextureFilteringMode)( tabDef.m_magFilter->currentIndex() );
      stageSettings.m_mipFilter = (TextureFilteringMode)( tabDef.m_mipFilter->currentIndex() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void PixelShaderEditor::onScriptModified()
{
   m_docModified = true;
}

///////////////////////////////////////////////////////////////////////////////

void PixelShaderEditor::synchronize()
{
   m_shader.setScript( m_scriptEditor->toPlainText().toStdString() );
   initializeTextureStagesTab();
}

///////////////////////////////////////////////////////////////////////////////

void PixelShaderEditor::importFrom()
{
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   std::string fsRootDir = fs.getCurrRoot();
   QString fileName = QFileDialog::getOpenFileName( this, "Import fragment shader from", fsRootDir.c_str() );
   if ( fileName.isEmpty() )
   {
      // no file selected
      return;
   }

   std::string relPath = fs.toRelativePath( fileName.toStdString() );
   File* file = fs.open( relPath, std::ios_base::in );

   // set the new contents in the editor and the shader
   StreamBuffer<char> shaderScript( *file );
   m_scriptEditor->setPlainText( shaderScript.getBuffer() );
   synchronize();

   // cleanup
   delete file;
}

///////////////////////////////////////////////////////////////////////////////

void PixelShaderEditor::exportTo()
{
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   std::string fsRootDir = fs.getCurrRoot();
   QString fileName = QFileDialog::getSaveFileName( this, "Export fragment shader from", fsRootDir.c_str() );
   if ( fileName.isEmpty() )
   {
      // no file selected
      return;
   }

   std::string relPath = fs.toRelativePath( fileName.toStdString() );
   File* file = fs.open( relPath, std::ios_base::out );
   file->writeString( m_shader.getScript().c_str() );
   file->flush();
   delete file;
}

///////////////////////////////////////////////////////////////////////////////

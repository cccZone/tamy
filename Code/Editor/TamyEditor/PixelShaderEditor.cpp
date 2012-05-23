#include "PixelShaderEditor.h"
#include "ShaderSyntaxHighlighter.h"
#include "tamyeditor.h"
#include "core.h"
#include "core-Renderer.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QTextBrowser>
#include <QMessageBox.h>
#include <QTextCursor>
#include <QTabWidget>
#include <QComboBox>


///////////////////////////////////////////////////////////////////////////////

PixelShaderEditor::PixelShaderEditor( PixelShader& shader )
   : m_shader( shader )
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
   m_highlighter = new ShaderSyntaxHighlighter( m_ui.scriptEditor->document() );

   // set the editor up
   m_ui.scriptEditor->setPlainText( m_shader.getScript().c_str() );
   m_ui.scriptEditor->setTabStopWidth( 15 );
   m_ui.scriptEditor->setFont( QFont( "Courier", 12 ) );
   m_ui.scriptEditor->setCurrentFont( QFont( "Courier", 12 ) );
   m_ui.scriptEditor->setAcceptRichText( false );
   m_ui.scriptEditor->setLineWrapMode( QTextEdit::NoWrap );
   connect( m_ui.scriptEditor, SIGNAL( textChanged() ), this, SLOT( onScriptModified() ) );
   connect( m_ui.scriptEditor, SIGNAL( cursorPositionChanged() ), this, SLOT( onTextCursorMoved() ) );
   m_docModified = false;

   // set the properties
   {
      const PixelShaderParams& params = m_shader.getParams();

      m_ui.cullingMode->setCurrentIndex( params.m_cullingMode - 1 );
      connect( m_ui.cullingMode, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

      m_ui.zBufferEnabled->setChecked( params.m_useZBuffer );
      connect( m_ui.zBufferEnabled, SIGNAL( toggled( bool ) ), this, SLOT( onParamChange() ) );

      m_ui.zBufferWriteEnabled->setChecked( params.m_writeToZBuffer );
      connect( m_ui.zBufferWriteEnabled, SIGNAL( toggled( bool ) ), this, SLOT( onParamChange() ) );
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
      connect( actionUndo, SIGNAL( triggered() ), m_ui.scriptEditor, SLOT( undo() ) );

      QAction* actionRedo = new QAction( QIcon( iconsDir + tr( "/redo.png" ) ), tr( "Redo" ), toolbar );
      actionRedo->setShortcut( QKeySequence( tr( "Ctrl+Y" ) ) );
      toolbar->addAction( actionRedo );
      connect( actionRedo, SIGNAL( triggered() ), m_ui.scriptEditor, SLOT( redo() ) );

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

   delete m_highlighter;
   m_highlighter = NULL;
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

   std::string shaderContents = m_ui.scriptEditor->toPlainText().toStdString();
   if ( shaderContents.empty() )
   {
      m_ui.compilationOutput->setText( "No code to compile" );
      return true;
   }

   ShaderCompiler compiler;
   bool status = compiler.compilePixelShader( shaderContents, m_shader.getEntryFunctionName().c_str() );

   if ( status )
   {
      m_ui.compilationOutput->setText( "Compilation successful" );
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

   params.m_cullingMode = (CullingMode)( m_ui.cullingMode->currentIndex() + 1 );
   params.m_useZBuffer = m_ui.zBufferEnabled->isChecked();
   params.m_writeToZBuffer = m_ui.zBufferWriteEnabled->isChecked();
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

void PixelShaderEditor::onTextCursorMoved()
{
   QTextCursor cursor = m_ui.scriptEditor->textCursor();

   char tmpStr[256];
   sprintf( tmpStr, "Row: %d, Col: %d", cursor.blockNumber(), cursor.columnNumber() );
   m_ui.statusBar->setText( tmpStr );
}

///////////////////////////////////////////////////////////////////////////////

void PixelShaderEditor::synchronize()
{
   m_shader.setScript( m_ui.scriptEditor->toPlainText().toStdString() );
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
   m_ui.scriptEditor->setPlainText( shaderScript.getBuffer() );
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

#include "MaterialEditor.h"
#include "MaterialSyntaxHighlighter.h"
#include "tamyeditor.h"
#include "core.h"
#include "core-Renderer.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox.h>
#include <QCloseEvent>


///////////////////////////////////////////////////////////////////////////////

MaterialEditor::MaterialEditor( PixelShader& shader )
: QMainWindow( NULL, 0 )
, m_shader( shader )
, m_highlighter( NULL )
, m_resourceMgr( NULL )
, m_renderer( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

MaterialEditor::~MaterialEditor()
{
   delete m_highlighter;
   m_highlighter = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEditor::initialize( TamyEditor& mgr )
{
   m_resourceMgr = &mgr.requestService< ResourcesManager >();
   m_renderer = &mgr.requestService< Renderer >();

   ASSERT( m_resourceMgr != NULL );
   ASSERT( m_renderer != NULL );

   m_ui.setupUi( this );
   m_highlighter = new MaterialSyntaxHighlighter( m_ui.scriptEditor->document() );

   // set the editor up
   m_ui.scriptEditor->setPlainText( m_shader.getScript().c_str() );
   m_ui.scriptEditor->setTabStopWidth( 15 );
   connect( m_ui.scriptEditor, SIGNAL( textChanged() ), this, SLOT( onScriptModified() ) );
   m_docModified = false;

   // set the propersties
   const PixelShaderParams& params = m_shader.getParams();

   m_ui.cullingMode->setCurrentIndex( params.m_cullingMode - 1 );
   connect( m_ui.cullingMode, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

   m_ui.zBufferEnabled->setChecked( params.m_useZBuffer );
   connect( m_ui.zBufferEnabled, SIGNAL( toggled( bool ) ), this, SLOT( onParamChange() ) );

   m_ui.zBufferWriteEnabled->setChecked( params.m_writeToZBuffer );
   connect( m_ui.zBufferWriteEnabled, SIGNAL( toggled( bool ) ), this, SLOT( onParamChange() ) );

   m_ui.uAddressMode->setCurrentIndex( params.m_addressU - 1 );
   connect( m_ui.uAddressMode, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

   m_ui.vAddressMode->setCurrentIndex( params.m_addressV - 1 );
   connect( m_ui.vAddressMode, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

   m_ui.wAddressMode->setCurrentIndex( params.m_addressW - 1 );
   connect( m_ui.wAddressMode, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

   m_ui.minFilter->setCurrentIndex( params.m_minFilter );
   connect( m_ui.minFilter, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

   m_ui.magFilter->setCurrentIndex( params.m_magFilter );
   connect( m_ui.magFilter, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

   m_ui.mipFilter->setCurrentIndex( params.m_mipFilter );
   connect( m_ui.mipFilter, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onParamChange() ) );

   // set the menu
   QString iconsDir = m_resourceMgr->getFilesystem().getShortcut( "editorIcons" ).c_str();
   QToolBar& toolBar = mgr.getToolBar();

   QMenu* fileMenu = m_ui.menubar->addMenu( "File" );
   {
      QAction* actionLoadFromFile = new QAction( QIcon( iconsDir + tr( "/openFile.png" ) ), tr( "Load from file" ), fileMenu );
      actionLoadFromFile->setShortcut( QKeySequence( tr( "Ctrl+O" ) ) );
      fileMenu->addAction( actionLoadFromFile );
      m_ui.toolBar->addAction( actionLoadFromFile );
      connect( actionLoadFromFile, SIGNAL( triggered() ), this, SLOT( loadFromFile() ) );

      QAction* actionSave = new QAction( QIcon( iconsDir + tr( "/saveFile.png" ) ), tr( "Save" ), fileMenu );
      actionSave->setShortcut( QKeySequence( tr( "Ctrl+S" ) ) );
      fileMenu->addAction( actionSave );
      m_ui.toolBar->addAction( actionSave );
      connect( actionSave, SIGNAL( triggered() ), this, SLOT( save() ) );

      QAction* separator = new QAction( fileMenu );
      separator->setSeparator( true );
      fileMenu->addAction( separator );
      m_ui.toolBar->addAction( separator );

      QAction* actionExit = new QAction( QIcon( iconsDir + tr( "/quit.png" ) ), tr( "Exit" ), this );
      actionExit->setShortcut( QKeySequence( tr( "Ctrl+Q" ) ) );
      fileMenu->addAction( actionExit );
      connect( actionExit, SIGNAL( triggered() ), this, SLOT( close() ) );
   }

   QMenu* editMenu = m_ui.menubar->addMenu( "Edit" );
   {
      QAction* actionUndo = new QAction( QIcon( iconsDir + tr( "/undo.png" ) ), tr( "Undo" ), editMenu );
      actionUndo->setShortcut( QKeySequence( tr( "Ctrl+Z" ) ) );
      editMenu->addAction( actionUndo );
      connect( actionUndo, SIGNAL( triggered() ), m_ui.scriptEditor, SLOT( undo() ) );

      QAction* actionRedo = new QAction( QIcon( iconsDir + tr( "/redo.png" ) ), tr( "Redo" ), editMenu );
      actionRedo->setShortcut( QKeySequence( tr( "Ctrl+Y" ) ) );
      editMenu->addAction( actionRedo );
      connect( actionRedo, SIGNAL( triggered() ), m_ui.scriptEditor, SLOT( redo() ) );
   }

   // connect the menu commands
   QMenu* buildMenu = m_ui.menubar->addMenu( "Build" );
   {
      QAction* actionCompile = new QAction( QIcon( iconsDir + tr( "/play.png" ) ), tr( "Compile" ), buildMenu );
      actionCompile->setShortcut( QKeySequence( tr( "F5" ) ) );
      buildMenu->addAction( actionCompile );
      m_ui.toolBar->addAction( actionCompile );
      connect( actionCompile, SIGNAL( triggered() ), this, SLOT( compile() ) );
   }

   show();
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEditor::closeEvent( QCloseEvent *event )
{
   if ( m_docModified )
   {
      int choice = QMessageBox::question( this, tr("Save"), tr("Would you like to save your changes?"), QMessageBox::Yes | QMessageBox::No );
      if ( choice == QMessageBox::Yes )
      {
         save();
      }
   }

   // accept the event
   event->accept();

   delete this;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEditor::loadFromFile()
{
   if ( !m_resourceMgr )
   {
      QMessageBox::warning( this, tr("Editor error"), tr("Resources manager not available"), QMessageBox::Ok );
      return;
   }

   const Filesystem& fs = m_resourceMgr->getFilesystem();
   std::string rootDir = fs.getCurrRoot();
   std::string filter( "Pixel shader files (*.psh; *.fx; *.txt)" );

   QString fullFileName = QFileDialog::getOpenFileName( this, tr("Load pixel shader code"), rootDir.c_str(), filter.c_str() );

   if ( fullFileName.isEmpty() == true ) 
   {
      // no file was selected or user pressed 'cancel'
      return;
   }

   // once the file is open, extract the directory name
   std::string fileName = fs.toRelativePath( fullFileName.toStdString() );
   m_shader.loadFromFile( fs, fileName, "main" );

   m_ui.scriptEditor->setPlainText( m_shader.getScript().c_str() );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEditor::save()
{
   synchronize();
   m_shader.saveResource();
   m_docModified = false;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEditor::compile()
{
   if ( !m_renderer )
   {
      QMessageBox::warning( this, tr("Editor error"), tr("Renderer not available"), QMessageBox::Ok );
      return;
   }

   try
   {
      synchronize();
      m_renderer->implement< PixelShader >( m_shader );
   }
   catch ( std::exception& ex )
   {
      QMessageBox::warning( this, tr("Compilation error"), ex.what(), QMessageBox::Ok );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEditor::onParamChange()
{
   PixelShaderParams& params = m_shader.getParams();

   params.m_cullingMode = (D3DCULL)( m_ui.cullingMode->currentIndex() + 1 );
   params.m_useZBuffer = m_ui.zBufferEnabled->isChecked();
   params.m_writeToZBuffer = m_ui.zBufferWriteEnabled->isChecked();
   params.m_addressU = (D3DTEXTUREADDRESS)( m_ui.uAddressMode->currentIndex() + 1 );
   params.m_addressV = (D3DTEXTUREADDRESS)( m_ui.vAddressMode->currentIndex() + 1 );
   params.m_addressW = (D3DTEXTUREADDRESS)( m_ui.wAddressMode->currentIndex() + 1 );
   params.m_minFilter = (D3DTEXTUREFILTERTYPE)( m_ui.minFilter->currentIndex() );
   params.m_magFilter = (D3DTEXTUREFILTERTYPE)( m_ui.magFilter->currentIndex() );
   params.m_mipFilter = (D3DTEXTUREFILTERTYPE)( m_ui.mipFilter->currentIndex() );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEditor::onScriptModified()
{
   m_docModified = true;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEditor::synchronize()
{
   m_shader.setScript( m_ui.scriptEditor->toPlainText().toStdString() );
}

///////////////////////////////////////////////////////////////////////////////

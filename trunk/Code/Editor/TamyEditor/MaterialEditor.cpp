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
   : m_shader( shader )
   , m_highlighter( NULL )
   , m_resourceMgr( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

MaterialEditor::~MaterialEditor()
{
   delete m_highlighter;
   m_highlighter = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEditor::onInitialize()
{
   m_resourceMgr = &ResourcesManager::getInstance();

   ASSERT( m_resourceMgr != NULL );

   // setup ui
   m_ui.setupUi( this );
   m_highlighter = new MaterialSyntaxHighlighter( m_ui.scriptEditor->document() );

   // set the editor up
   m_ui.scriptEditor->setPlainText( m_shader.getScript().c_str() );
   m_ui.scriptEditor->setTabStopWidth( 15 );
   connect( m_ui.scriptEditor, SIGNAL( textChanged() ), this, SLOT( onScriptModified() ) );
   m_docModified = false;

   // set the properties
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

void MaterialEditor::onDeinitialize()
{
   if ( m_docModified )
   {
      int choice = QMessageBox::question( this, tr("Save"), tr("Would you like to save your changes?"), QMessageBox::Yes | QMessageBox::No );
      if ( choice == QMessageBox::Yes )
      {
         save();
      }
   }
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
   try
   {
      synchronize();
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

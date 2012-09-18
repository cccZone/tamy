#include "FragmentShaderEditor.h"
#include "ShaderSyntaxHighlighter.h"
#include "tamyeditor.h"
#include "core.h"
#include "core-Renderer.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QTextBrowser>
#include <QMessageBox.h>
#include <QTextCursor>
#include "TextEditWidget.h"


///////////////////////////////////////////////////////////////////////////////

FragmentShaderEditor::FragmentShaderEditor( FragmentShader& shader )
   : m_shader( shader )
   , m_scriptEditor( NULL )
   , m_resourceMgr( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

FragmentShaderEditor::~FragmentShaderEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void FragmentShaderEditor::onInitialize()
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

   show();
}

///////////////////////////////////////////////////////////////////////////////

void FragmentShaderEditor::onDeinitialize( bool saveProgress )
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

void FragmentShaderEditor::save()
{
   synchronize();
   m_shader.saveResource();
   m_docModified = false;
}

///////////////////////////////////////////////////////////////////////////////

void FragmentShaderEditor::onScriptModified()
{
   m_docModified = true;
}

///////////////////////////////////////////////////////////////////////////////

void FragmentShaderEditor::synchronize()
{
   m_shader.setScript( m_scriptEditor->toPlainText().toStdString() );
}

///////////////////////////////////////////////////////////////////////////////

void FragmentShaderEditor::importFrom()
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
   StreamBuffer<char> shaderScript( *file );
   m_shader.setScript( shaderScript.getBuffer() );
   delete file;

   // set the new contents in the editor
   m_scriptEditor->setPlainText( m_shader.getScript().c_str() );
}

///////////////////////////////////////////////////////////////////////////////

void FragmentShaderEditor::exportTo()
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

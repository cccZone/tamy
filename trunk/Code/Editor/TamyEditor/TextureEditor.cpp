#include "TextureEditor.h"
#include "TamyEditor.h"
#include "core.h"
#include "core-Renderer/Texture.h"
#include "FSNodeMimeData.h"
#include "DropArea.h"
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QPixmap>
#include <QImage>
#include "QPropertiesView.h"


///////////////////////////////////////////////////////////////////////////////

TextureEditor::TextureEditor( Texture& texture )
   : m_texture( texture )
   , m_image( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

TextureEditor::~TextureEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void TextureEditor::onInitialize()
{
   TamyEditor& mainEditor = TamyEditor::getInstance();


   // initialize user interface
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   QString iconsDir = resMgr.getFilesystem().getShortcut( "editorIcons" ).c_str();

   // setup the main layout
   QVBoxLayout* mainLayout = new QVBoxLayout( this );
   mainLayout->setContentsMargins(0, 0, 0, 0);
   setLayout( mainLayout );

   // add the toolbar
   {
      QToolBar* toolBar = new QToolBar( this );
      mainLayout->addWidget( toolBar );

      QAction* actionSaveScene = new QAction( QIcon( iconsDir + tr( "/saveFile.png" ) ), tr( "Save Scene" ), toolBar );
      toolBar->addAction( actionSaveScene );
      connect( actionSaveScene, SIGNAL( triggered() ), this, SLOT( saveTexture() ) );
   }

   // edit frame
   {
      QFrame* editFrame = new QFrame( this );
      QHBoxLayout* editFrameLayout = new QHBoxLayout( editFrame );
      editFrame->setLayout( editFrameLayout );
      mainLayout->addWidget( editFrame );

      // properties viewer
      {
         QPropertiesView* propertiesView = new QPropertiesView();
         editFrameLayout->addWidget( propertiesView, 0 );
         m_texture.viewProperties( *propertiesView );
      }

      // add the image viewer widget
      {
         m_image = new DropArea( editFrame, new FSNodeMimeData( m_imagePaths ) );
         editFrameLayout->addWidget( m_image, 1 );
         m_image->setBackgroundRole( QPalette::Base );
         m_image->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
         m_image->setScaledContents( true );

         connect( m_image, SIGNAL( changed() ), this, SLOT( onTextureSet() ) );
      }
   }

   // initialize the contents
   refreshImage();
}

///////////////////////////////////////////////////////////////////////////////

void TextureEditor::onDeinitialize( bool saveProgress )
{
   if ( saveProgress )
   {
      m_texture.saveResource();
   }

   m_image = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TextureEditor::refreshImage()
{
   const Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   std::string absolutePath = fs.toAbsolutePath( m_texture.getTextureName() );

   QImage image( absolutePath.c_str() );
   m_image->setPixmap( QPixmap::fromImage( image ) );
}

///////////////////////////////////////////////////////////////////////////////

void TextureEditor::onTextureSet()
{
   const Filesystem& fs = ResourcesManager::getInstance().getFilesystem();

   unsigned int count = m_imagePaths.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      std::string absolutePath = fs.toAbsolutePath( m_imagePaths[i] );
      QImage image( absolutePath.c_str() );
      if ( image.isNull() == false )
      {
         m_texture.setTextureName( m_imagePaths[i] );
         break;
      }
   }
   m_imagePaths.clear();

   refreshImage();
}

///////////////////////////////////////////////////////////////////////////////

void TextureEditor::saveTexture()
{
   m_texture.saveResource();
}

///////////////////////////////////////////////////////////////////////////////

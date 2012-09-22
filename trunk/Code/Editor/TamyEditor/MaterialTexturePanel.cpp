#include "MaterialTexturePanel.h"
#include "core/ResourcesManager.h"
#include "core-Renderer/Texture.h"
#include "ResourceDropArea.h"
#include "DropArea.h"
#include "FSNodeMimeData.h"
#include <QVBoxLayout>
#include <QImage>
#include <QPixmap>


///////////////////////////////////////////////////////////////////////////////

MaterialTexturePanel::MaterialTexturePanel( QWidget* parentWidget, MaterialTextures usage, MaterialInstance& materialInstance )
   : QFrame( parentWidget )
   , m_textureUsage( usage )
   , m_materialInstance( materialInstance )
{
   QVBoxLayout* layout = new QVBoxLayout( this );
   setLayout( layout );
   layout->setMargin( 0 );
   layout->setSpacing( 5 );

   // texture file path drop area
   FilePath texturePath;
   {
      Texture* texture = m_materialInstance.getTexture( usage );
      if ( texture )
      {
         texturePath = texture->getFilePath();
      }
      m_texturePath = new ResourceDropArea( this, texturePath );
      layout->addWidget( m_texturePath, 0, Qt::AlignTop );
   }

   // texture image display and drop area
   {
      m_imageDropArea = new DropArea( this, new FSNodeMimeData( m_imagesPaths ) );
      layout->addWidget( m_imageDropArea, 1 );
      m_imageDropArea->setBackgroundRole( QPalette::Base );
      m_imageDropArea->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
      m_imageDropArea->setScaledContents( true );

      connect( m_imageDropArea, SIGNAL( changed() ), this, SLOT( onImageSet() ) );
   }

   refreshImage();
}

///////////////////////////////////////////////////////////////////////////////

void MaterialTexturePanel::refreshImage()
{

   Texture* texture = m_materialInstance.getTexture( m_textureUsage );
   if ( texture )
   {
      const Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
      std::string absolutePath = fs.toAbsolutePath( texture->getFilePath() );

      QImage image( absolutePath.c_str() );
      m_imageDropArea->setPixmap( QPixmap::fromImage( image ) );
   }
   else
   {
      m_imageDropArea->setPixmap( QPixmap() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialTexturePanel::onTexturePathChanged( const FilePath& texturePath )
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   Texture* texture = resMgr.create< Texture >( texturePath, true );
   m_materialInstance.setTexture( m_textureUsage, texture );

   refreshImage();
}

///////////////////////////////////////////////////////////////////////////////

void MaterialTexturePanel::onImageSet()
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   unsigned int count = m_imagesPaths.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      FilePath texturePath( m_imagesPaths[i] );
      Texture* texture = resMgr.create< Texture >( texturePath, true );
      if ( texture )
      {
         m_materialInstance.setTexture( m_textureUsage, texture );
         m_texturePath->setFilePath( texturePath );
         refreshImage();
         break;
      }
   }
   m_imagesPaths.clear();
}

///////////////////////////////////////////////////////////////////////////////

#include "ResourceDropArea.h"
#include "DropArea.h"
#include "FSNodeMimeData.h"
#include <QPushButton>
#include <QHBoxLayout>
#include "core/ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

ResourceDropArea::ResourceDropArea( QWidget* parentWidget )
   : QFrame( parentWidget )
{
   QHBoxLayout* layout = new QHBoxLayout( this );
   layout->setSpacing(0);
   layout->setMargin(0);
   setLayout( layout );

   // a field that shows the resource
   m_resourceName = new DropArea( this, new FSNodeMimeData( m_paths ) );
   connect( m_resourceName, SIGNAL( changed() ), this, SLOT( valChanged() ) );
   m_resourceName->setMinimumSize( 20, 20 );
   m_resourceName->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
   layout->addWidget( m_resourceName );

   // a button for NULLing the resource
   ResourcesManager& rm = ResourcesManager::getInstance();
   std::string iconsDir = rm.getFilesystem().getShortcut( "editorIcons" );

   QPushButton* eraseButton = new QPushButton( this );
   eraseButton->setIcon( QIcon( ( iconsDir + "quit.png" ).c_str() ) );
   eraseButton->setToolTip( "Reset resource value" );
   connect( eraseButton, SIGNAL( clicked( bool ) ), this, SLOT( valErased( bool ) ) );
   eraseButton->setMinimumSize( 20, 20 );
   eraseButton->setMaximumSize( 20, 20 );
   eraseButton->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
   layout->addWidget( eraseButton );

   refreshPropertyName();
}

///////////////////////////////////////////////////////////////////////////////

void ResourceDropArea::refreshPropertyName()
{
   if ( !m_paths.empty() )
   {
      m_path.set( m_paths[0] );
      m_resourceName->setText( m_paths[0].c_str() );
   }
   else
   {
      m_path.set( "" );
      m_resourceName->setText( "<No resource>");
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourceDropArea::valChanged()
{
   refreshPropertyName();
   m_paths.clear();
}

///////////////////////////////////////////////////////////////////////////////

void ResourceDropArea::valErased( bool ) 
{
   m_paths.clear();
   refreshPropertyName();
}

///////////////////////////////////////////////////////////////////////////////

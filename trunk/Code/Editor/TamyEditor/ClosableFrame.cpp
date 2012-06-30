#include "ClosableFrame.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include "core/ResourcesManager.h"
#include "core/Filesystem.h"


///////////////////////////////////////////////////////////////////////////////

ClosableFrame::ClosableFrame( QWidget* parentWidget )
   : QFrame( parentWidget )
   , m_embeddedWidget( NULL )
{
   m_mainLayout = new QVBoxLayout( this );
   m_mainLayout->setSpacing( 0 );
   m_mainLayout->setMargin( 0 );
   setLayout( m_mainLayout );

   // close button bar
   {
      QFrame* closeButtonBar = new QFrame( this );
      m_mainLayout->addWidget( closeButtonBar, 0, Qt::AlignTop );
      QHBoxLayout* closeButtonBarLayout = new QHBoxLayout( closeButtonBar );
      closeButtonBarLayout->setSpacing( 0 );
      closeButtonBarLayout->setMargin( 0 );
      closeButtonBar->setLayout( closeButtonBarLayout );

      // add the spacer that's gonna push the close button to the right
      QSpacerItem* spacerItem = new QSpacerItem( 1000, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
      closeButtonBarLayout->addSpacerItem( spacerItem );
      
      // add the close button
      QPushButton* closeButton = new QPushButton( closeButtonBar );
      connect( closeButton, SIGNAL( clicked() ), this, SLOT( onClose() ) );

      QString iconsDir = ResourcesManager::getInstance().getFilesystem().getShortcut( "editorIcons" ).c_str();
      closeButton->setIcon( QIcon( iconsDir + "/closeButton.png" ) );

      closeButtonBarLayout->addWidget( closeButton );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ClosableFrame::onClose()
{
   emit onCloseRequest( this );
}

///////////////////////////////////////////////////////////////////////////////

void ClosableFrame::setWidget( QWidget* widget )
{
   if ( m_embeddedWidget )
   {
      m_mainLayout->removeWidget( m_embeddedWidget );

      m_embeddedWidget->setParent( NULL );
      m_embeddedWidget->deleteLater();
   }

   m_embeddedWidget = widget;
   m_mainLayout->addWidget( m_embeddedWidget, 1, Qt::AlignBottom );

   // adjust the size of the frame
   setMaximumSize( m_embeddedWidget->maximumSize() );
}

///////////////////////////////////////////////////////////////////////////////

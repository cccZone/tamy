#include "ResourcePropertyEditor.h"
#include "DropArea.h"
#include "FSNodeMimeData.h"
#include "core.h"
#include "tamyeditor.h"
#include <QPushButton>


///////////////////////////////////////////////////////////////////////////////

ResourcePropertyEditor::ResourcePropertyEditor( TEditableProperty< Resource* >* property )
: QPropertyEditor( property->getLabel().c_str() )
, m_property( property )
, m_resourceName( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

ResourcePropertyEditor::~ResourcePropertyEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void ResourcePropertyEditor::onInitialize()
{
   QWidget* frame = new QWidget( this );
   QHBoxLayout* layout = new QHBoxLayout( frame );
   layout->setSpacing(0);
   layout->setMargin(0);
   frame->setLayout( layout );
   addWidget( frame );

   // a field that shows the resource
   m_resourceName = new DropArea( frame, new FSNodeMimeData( m_paths ) );
   connect( m_resourceName, SIGNAL( changed() ), this, SLOT( valChanged() ) );
   m_resourceName->setMinimumSize( 20, 20 );
   m_resourceName->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
   layout->addWidget( m_resourceName );

   // a button for NULLing the resource
   ResourcesManager& rm = ResourcesManager::getInstance();
   std::string iconsDir = rm.getFilesystem().getShortcut( "editorIcons" );

   QPushButton* eraseButton = new QPushButton( frame );
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

void ResourcePropertyEditor::refreshPropertyName()
{
   Resource* res = m_property->get();
   if ( res )
   {
      m_resourceName->setText( res->getFilePath().c_str() );
   }
   else
   {
      m_resourceName->setText( "<No resource>");
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcePropertyEditor::valChanged()
{
   Resource* newVal = NULL;
   if ( !m_paths.empty() )
   {
      ResourcesManager& rm = ResourcesManager::getInstance();
      newVal = &rm.create( m_paths[ 0 ] );

      // verify that the types match
      Class newTypeClass = newVal->getVirtualClass();
      Class acceptableType = m_property->getType();
      if ( !newTypeClass.isA( acceptableType ) )
      {
         newVal = NULL;
      }
   }

   if ( newVal )
   {
      m_property->set( newVal );
      refreshPropertyName();
   }

   m_paths.clear();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcePropertyEditor::valErased( bool ) 
{
   m_property->set( NULL );
   refreshPropertyName();
}

///////////////////////////////////////////////////////////////////////////////

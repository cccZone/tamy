#include "ResourcePropertyEditor.h"
#include "DropArea.h"
#include "FSNodeMimeData.h"
#include "core.h"
#include "tamyeditor.h"
#include <QPushButton>


///////////////////////////////////////////////////////////////////////////////

ResourcePropertyEditor::ResourcePropertyEditor( Resource*& property, 
                                                const Class& acceptableType,
                                                const std::string& label )
: QPropertyEditor( label.c_str() )
, m_property( property )
, m_acceptableType( acceptableType )
, m_resourceName( NULL )
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
   ResourcesManager& rm = getServicesMgr().requestService< ResourcesManager >();
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
   if ( m_property )
   {
      m_resourceName->setText( m_property->getResourceName().c_str() );
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
      ResourcesManager& rm = getServicesMgr().requestService< ResourcesManager >();
      newVal = &rm.create( m_paths[ 0 ] );

      // verify that the types match
      Class newTypeClass = newVal->getVirtualClass();
      if ( !m_acceptableType.isA( newTypeClass ) )
      {
         newVal = NULL;
      }
   }

   if ( newVal )
   {
      m_property = newVal;
      refreshPropertyName();
   }

   m_paths.clear();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcePropertyEditor::valErased( bool ) 
{
   m_property = NULL;
   refreshPropertyName();
}

///////////////////////////////////////////////////////////////////////////////

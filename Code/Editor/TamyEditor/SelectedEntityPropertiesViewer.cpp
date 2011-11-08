#include "SelectedEntityPropertiesViewer.h"
#include "QPropertiesView.h"
#include "core-MVC/Entity.h"


///////////////////////////////////////////////////////////////////////////////

SelectedEntityPropertiesViewer::SelectedEntityPropertiesViewer( QWidget* parent )
   : QScrollArea( parent )
   , m_selectedEntityProperties( NULL )
   , m_displayedEntity( NULL )
{
   setWidgetResizable( true );
}

///////////////////////////////////////////////////////////////////////////////

void SelectedEntityPropertiesViewer::onEntitySelected( Entity& entity )
{
   if ( &entity == m_displayedEntity )
   {
      // same entity was selected as the one we already have in the view - don't do a thing
      return;
   }
   m_displayedEntity = &entity;

   if ( m_selectedEntityProperties != NULL )
   {
      setWidget( NULL );
      delete m_selectedEntityProperties;
   }

   m_selectedEntityProperties = new QPropertiesView();
   setWidget( m_selectedEntityProperties );
   entity.viewProperties( *m_selectedEntityProperties );
}

///////////////////////////////////////////////////////////////////////////////

void SelectedEntityPropertiesViewer::onEntityDeselected( Entity& entity )
{
   if ( &entity != m_displayedEntity )
   {
      // a different entity from the one we are viewing was deselected - don't do a thing
      return;
   }
   m_displayedEntity = NULL;

   if ( m_selectedEntityProperties != NULL )
   {
      setWidget( NULL );
      delete m_selectedEntityProperties;
      m_selectedEntityProperties = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

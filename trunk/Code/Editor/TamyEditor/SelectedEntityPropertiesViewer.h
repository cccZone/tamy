/// @file   TamyEditor/SelectedEntityPropertiesViewer.h
/// @brief  a helper widget that displays the properties of the most recently selected entity
#pragma once

#include <QScrollArea>
#include "SelectionManagerListener.h"


///////////////////////////////////////////////////////////////////////////////

class QPropertiesView;
class Entity;

///////////////////////////////////////////////////////////////////////////////

/**
 * A helper widget that displays the properties of the most recently selected entity.
 */
class SelectedEntityPropertiesViewer : public QScrollArea, public SelectionManagerListener
{
   Q_OBJECT

private:
   Entity*                    m_displayedEntity;
   QPropertiesView*           m_selectedEntityProperties;

public:
   /**
    * Constructor.
    *
    * @param parent
    */
   SelectedEntityPropertiesViewer( QWidget* parent );

   // -------------------------------------------------------------------------
   // SelectionManager implementation
   // -------------------------------------------------------------------------
   void onEntitySelected( Entity& entity );
   void onEntityDeselected( Entity& entity );
};

///////////////////////////////////////////////////////////////////////////////

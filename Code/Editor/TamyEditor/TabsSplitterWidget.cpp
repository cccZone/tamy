#include "TabsSplitterWidget.h"


///////////////////////////////////////////////////////////////////////////////

TabsSplitterWidget::TabsSplitterWidget( Qt::Orientation splitOrientation, QWidget* parentWidget )
   : QSplitter( splitOrientation, parentWidget )
{
   setHandleWidth( 3 );
   setOpaqueResize( false );
}

///////////////////////////////////////////////////////////////////////////////

void TabsSplitterWidget::collectChildren( std::list< TreeTabNode* >& outChildren )
{
   const QObjectList& childrenList = this->children();
   uint count = childrenList.count();
   for ( uint i = 0; i < count; ++i )
   {
      QObject* child = childrenList[i];
      TreeTabNode* node = dynamic_cast< TreeTabNode* >( child );
      if ( node )
      {
         outChildren.push_back( node );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

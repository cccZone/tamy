#ifndef _GRAPH_WIDGET_H
#error "This file can only be included from GraphWidget.h"
#else


///////////////////////////////////////////////////////////////////////////////

template< typename T >
void GraphWidget::associate( GraphBlock::Shape shape, const QColor& bgColor )
{
   Class nodeType = Class::createClass< T >();
   NodeAssociacion* associacion = findAssociacion( nodeType );
   if ( !associacion )
   {
      m_associacions.push_back( NodeAssociacion( nodeType ) );
      associacion = &m_associacions.back();
   }

   // even if an associacion already exists, we're gonna update it
   associacion->m_shape = shape;
   associacion->m_bgColor = bgColor;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_WIDGET_H

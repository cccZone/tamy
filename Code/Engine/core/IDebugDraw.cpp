#include "core/IDebugDraw.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

void DebugScene::add( IDebugDrawable& drawable )
{
   // check if the drawable isn't already present in the drawables collection
   if ( std::find( m_drawables.begin(), m_drawables.end(), &drawable ) == m_drawables.end() )
   {
      // ok - it's not there - add it
      m_drawables.push_back( &drawable );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DebugScene::remove( IDebugDrawable& drawable )
{
   std::vector< IDebugDrawable* >::iterator it = std::find( m_drawables.begin(), m_drawables.end(), &drawable );
   if ( it != m_drawables.end() )
   {
      m_drawables.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DebugScene::clear()
{
   m_drawables.clear();
}

///////////////////////////////////////////////////////////////////////////////

void DebugScene::onDebugRender( IDebugDraw& renderer ) const
{
   unsigned int count = m_drawables.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_drawables[i]->onDebugRender( renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////

#include "QueryableGeometry.h"
#include "core-Renderer.h"
#include "core.h"
#include "SceneQueries.h"
#include "SceneQueryEffect.h"


///////////////////////////////////////////////////////////////////////////////

QueryableGeometry::QueryableGeometry( Geometry& geometry )
: m_geometry( geometry )
, m_effect( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void QueryableGeometry::initialize( SceneQueryEffect& effect )
{
   m_effect = &effect;
}

///////////////////////////////////////////////////////////////////////////////

void QueryableGeometry::render()
{
   if ( !m_effect || !m_geometry.hasGeometry() || !m_geometry.isVisible() )
   {
      return;
   }

   m_effect->setObject( SceneQueries::ptrToVec( &m_geometry ) );
   m_effect->render( m_geometry );
}

///////////////////////////////////////////////////////////////////////////////

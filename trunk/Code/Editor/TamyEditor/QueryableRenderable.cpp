#include "QueryableRenderable.h"
#include "core-Renderer.h"
#include "core.h"
#include "SceneQueries.h"
#include "SceneQueryEffect.h"


///////////////////////////////////////////////////////////////////////////////

QueryableRenderable::QueryableRenderable( Renderable& renderable )
: m_renderable( renderable )
, m_effect( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void QueryableRenderable::initialize( SceneQueryEffect& effect )
{
   m_effect = &effect;
}

///////////////////////////////////////////////////////////////////////////////

void QueryableRenderable::render()
{
   if ( !m_effect || !m_renderable.hasGeometry() || !m_renderable.isVisible() )
   {
      return;
   }

   m_effect->setObject( m_renderable.getGlobalMtx(), SceneQueries::ptrToVec( &m_renderable ) );
   m_effect->render( m_renderable.getGeometry() );
}

///////////////////////////////////////////////////////////////////////////////

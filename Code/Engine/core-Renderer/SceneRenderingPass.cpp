#include "core-Renderer\SceneRenderingPass.h"
#include "core-MVC\Model.h"
#include "core-Renderer\RenderingView.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

SceneRenderingPass::SceneRenderingPass( RenderTarget* renderTarget )
: RenderingPass( renderTarget )
{
   m_renderingView = new RenderingView();
   m_renderingView->setAttributeSorter( statesMgr() );
}

///////////////////////////////////////////////////////////////////////////////

SceneRenderingPass::~SceneRenderingPass()
{
   // detach all scenes from the views
   unsigned int count = m_renderedScenes.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_renderedScenes[i]->detach( *m_renderingView );
   }
   m_renderedScenes.clear();

   delete m_renderingView; m_renderingView = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SceneRenderingPass::addScene( Model& model )
{
   // check if this is a unique scene - we don't want to waste time
   // on rendering the same scene twice
   bool isAdded = std::find( m_renderedScenes.begin(), m_renderedScenes.end(), &model ) != m_renderedScenes.end();
   if ( isAdded )
   {
      return;
   }

   m_renderedScenes.push_back( &model );
   model.attach( *m_renderingView );
}

///////////////////////////////////////////////////////////////////////////////

void SceneRenderingPass::removeScene( Model& model )
{
   std::vector< Model* >::iterator it = std::find( m_renderedScenes.begin(), m_renderedScenes.end(), &model );
   bool isAdded = ( it != m_renderedScenes.end() );
   if ( !isAdded )
   {
      return;
   }

   m_renderedScenes.erase( it );
   model.detach( *m_renderingView );
}

///////////////////////////////////////////////////////////////////////////////

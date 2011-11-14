#include "SelectionRenderingPass.h"
#include "core-Renderer.h"
#include "SelectedEntityRepresentation.h"

// representations
#include "Gizmo.h"
#include "SelectedGeometry.h"


///////////////////////////////////////////////////////////////////////////////

SelectionRenderingPass::SelectionRenderingPass()
{
   // define associations
   associateAbstract< Geometry, SelectedGeometry >();
}

///////////////////////////////////////////////////////////////////////////////

SelectionRenderingPass::~SelectionRenderingPass()
{
   unsigned int count = m_representations.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_representations[i];
   }
   m_representations.clear();
}

///////////////////////////////////////////////////////////////////////////////

void SelectionRenderingPass::add( Entity& entity )
{
   // remove the previous representations
   unsigned int count = m_representations.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_representations[i];
   }
   m_representations.clear();

   // create new representations
   std::list< Entity* > m_bfsQueue;
   m_bfsQueue.push_back( &entity );
   while( !m_bfsQueue.empty() )
   {
      Entity* currEntity = m_bfsQueue.front();
      m_bfsQueue.pop_front();

      SelectedEntityRepresentation* representation = create( *currEntity );
      if ( representation )
      {
         m_representations.push_back( representation );
      }

      // get the entity's children, if there are any
      const Entity::Children& childEntities = currEntity->getEntityChildren();
      unsigned int childrenCount = childEntities.size();
      for ( unsigned int i = 0; i < childrenCount; ++i )
      {
         m_bfsQueue.push_back( childEntities[i] );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SelectionRenderingPass::remove( Entity& entity )
{
   unsigned int count = m_representations.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_representations[i];
   }
   m_representations.clear();
}

///////////////////////////////////////////////////////////////////////////////

void SelectionRenderingPass::initialize( Renderer& renderer )
{

}

///////////////////////////////////////////////////////////////////////////////

void SelectionRenderingPass::deinitialize( Renderer& renderer )
{

}

///////////////////////////////////////////////////////////////////////////////

void SelectionRenderingPass::render( Renderer& renderer )
{
   new ( renderer() ) RCActivateRenderTarget( NULL );

   unsigned int count = m_representations.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_representations[i]->render( renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////

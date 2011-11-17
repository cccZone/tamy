#include "core-Renderer\CompositeRenderingMechanism.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

CompositeRenderingMechanism::CompositeRenderingMechanism()
: m_renderer( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

CompositeRenderingMechanism::~CompositeRenderingMechanism() 
{
   reset(); 
   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::initialize( Renderer& renderer )
{
   if ( m_renderer )
   {
      throw std::logic_error( "Can't initialize the same mechanism twice" );
   }
   m_renderer = &renderer;

   // initialize all currently attached mechanisms
   unsigned int count = m_members.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_members[i]->m_mechanism->initialize( renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::deinitialize( Renderer& renderer )
{
   reset();

   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::add( const std::string& name, RenderingMechanism* mechanism, bool manage )
{
   if ( mechanism == NULL )
   {
      throw std::invalid_argument("NULL pointer instead a RenderingMechanism instance");
   }

   // check if the mechanism with this name already exists
   unsigned int count = m_members.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_members[i]->m_name == name )
      {
         // such mechanism already exists - replace it
         m_members[ i ]->m_mechanism->deinitialize( *m_renderer );
         if ( m_members[i]->m_release )
         {
            delete m_members[ i ];
         }
         m_members[ i ]->m_mechanism = mechanism;
         m_members[i]->m_release = manage;
         return;
      }
   }

   m_members.push_back( new MechDef( name, mechanism, manage ) );
   
   if ( m_renderer )
   {
      mechanism->initialize( *m_renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::remove( const std::string& name )
{
   // check if the mechanism with this name already exists
   unsigned int count = m_members.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_members[i]->m_name == name )
      {
         // deinitialize and release( if requested ) the mechanism
         m_members[ i ]->m_mechanism->deinitialize( *m_renderer );
         delete m_members[i];

         m_members.erase( m_members.begin() + i );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::reset()
{
   std::vector< MechDef* > members = m_members;
   m_members.clear();

   unsigned int count = members.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_renderer )
      {
         members[i]->m_mechanism->deinitialize( *m_renderer );
      }
      delete members[i];
   }
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::render( Renderer& renderer )
{
   unsigned int count = m_members.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_members[i]->m_mechanism->render( renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////

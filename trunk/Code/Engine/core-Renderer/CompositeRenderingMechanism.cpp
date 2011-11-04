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
   for (unsigned int i = 0; i < count; ++i)
   {
      if ( m_members[i] )
      {
         m_members[i]->initialize( renderer );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::deinitialize( Renderer& renderer )
{
   unsigned int count = m_members.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if ( m_members[i] )
      {
         m_members[i]->deinitialize( renderer );
         delete m_members[i];
      }
   }
   m_members.clear();
   m_mechanismsMap.clear();
   m_freeSlots.clear();

   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::add( const std::string& name, RenderingMechanism* mechanism )
{
   if ( mechanism == NULL )
   {
      throw std::invalid_argument("NULL pointer instead a RenderingMechanism instance");
   }

   MechanismsMap::iterator it = m_mechanismsMap.find( name );
   if ( it != m_mechanismsMap.end() )
   {
      // such mechanism already exists - replace it
      m_members[ it->second ]->deinitialize( *m_renderer );
      delete m_members[ it->second ];
      m_members[ it->second ] = mechanism;
   }
   else
   {
      int freeSpotIdx;
      if ( !m_freeSlots.empty() )
      {
         freeSpotIdx = m_freeSlots.front();
         m_freeSlots.pop_front();
      }
      else
      {
         freeSpotIdx = m_members.size();
         m_members.push_back( NULL );
      }

      // this is a brand new mechanism
      m_mechanismsMap.insert( std::make_pair( name, freeSpotIdx ) );
      m_members[freeSpotIdx] = mechanism;
   }

   if ( m_renderer )
   {
      mechanism->initialize( *m_renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::remove( const std::string& name )
{
   MechanismsMap::iterator it = m_mechanismsMap.find( name );
   if ( it != m_mechanismsMap.end() )
   {
      // such mechanism already exists - replace it
      m_members[ it->second ]->deinitialize( *m_renderer );
      delete m_members[ it->second ];
      m_members[ it->second ] = NULL;
      m_freeSlots.push_back( it->second );

      m_mechanismsMap.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::render( Renderer& renderer )
{
   unsigned int count = m_members.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if ( m_members[i] )
      {
         m_members[i]->render( renderer );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

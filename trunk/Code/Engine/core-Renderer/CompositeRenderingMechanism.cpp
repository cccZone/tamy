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
   unsigned int count = m_members.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_members[i];
   }
   m_members.clear();
   m_mechanismsMap.clear();

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
   for (unsigned int i = 0; i < count; ++i)
   {
      m_members[i]->initialize( renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::add( const std::string& name, RenderingMechanism* mechanism )
{
   if (mechanism == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a RenderingMechanism instance");
   }

   MechanismsMap::iterator it = m_mechanismsMap.find( name );
   if ( it != m_mechanismsMap.end() )
   {
      // such mechanism already exists - replace it
      delete m_members[ it->second ];
      m_members[ it->second ] = mechanism;
   }
   else
   {
      // this is a brand new mechanism
      m_mechanismsMap.insert( std::make_pair( name, m_members.size() ) );
      m_members.push_back( mechanism );
   }

   mechanism->initialize( *m_renderer );
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::render()
{
   unsigned int count = m_members.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_members[i]->render();
   }
}

///////////////////////////////////////////////////////////////////////////////

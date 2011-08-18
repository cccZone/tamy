#include "core-Renderer\LineSegments.h"
#include "core-Renderer\Renderer.h"
#include "core.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( LineSegments, GeometryResource, tls, AM_BINARY )
   PROPERTY( std::vector<LineSegment>, m_segments )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

LineSegments::LineSegments( const std::string& name )
: GeometryResource( name )
{
   D3DXMatrixIdentity( &m_identityMtx );
}

///////////////////////////////////////////////////////////////////////////////


void LineSegments::onResourceLoaded(ResourcesManager& mgr) 
{
   // calculate the bounding box
   m_bb = AABoundingBox();
   for (std::vector<LineSegment>::iterator it = m_segments.begin();
      it != m_segments.end(); ++it )
   {
      m_bb.include(it->start);
      m_bb.include(it->end);
   }
}

///////////////////////////////////////////////////////////////////////////////

void LineSegments::add(const LineSegment& segment)
{
   m_segments.push_back(segment);

   m_bb.include(segment.start);
   m_bb.include(segment.end);
}

///////////////////////////////////////////////////////////////////////////////

void LineSegments::remove(const LineSegment& segment)
{
   m_bb = AABoundingBox();

   for (std::vector<LineSegment>::iterator it = m_segments.begin();
        it != m_segments.end();)
   {
      if (*it == segment)
      {
         it = m_segments.erase(it);
      }
      else
      {
         m_bb.include(it->start);
         m_bb.include(it->end);
         ++it;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void LineSegments::clear()
{
   m_bb = AABoundingBox();
   m_segments.clear();
}

///////////////////////////////////////////////////////////////////////////////

void LineSegments::render( Renderer& renderer )
{
   new ( renderer() ) RCRenderLineSegments( *this );
}

///////////////////////////////////////////////////////////////////////////////

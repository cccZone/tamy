#include "core-Renderer\LineSegments.h"
#include "core-Renderer\Renderer.h"
#include "core.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE_NO_PARENT( LineSegments, tls, AM_BINARY )
   PARENT( GeometryResource )
   PROPERTY( Array<LineSegment>, m_segments )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

LineSegments::LineSegments( const FilePath& resourceName )
   : GeometryResource( resourceName )
   , m_boundsDirty( true )
{
}

///////////////////////////////////////////////////////////////////////////////

void LineSegments::calculateBoundingVolume()
{
   // calculate the bounding box
   m_bb = AABoundingBox();

   uint count = m_segments.size();
   for ( uint i = 0; i < count; ++i )
   {
      const LineSegment& seg = m_segments[i];
      m_bb.include( seg.start );
      m_bb.include( seg.end );
   }

   m_boundsDirty = false;
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& LineSegments::getBoundingVolume()
{
   if ( m_boundsDirty )
   {
      calculateBoundingVolume();
   }

   return m_bb;
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

   int count = m_segments.size();
   for ( int i = count - 1; i >= 0; --i )
   {
      const LineSegment& seg = m_segments[i];
      if ( seg == segment )
      {
         m_segments.remove( i );
      }
      else
      {
         m_bb.include( seg.start );
         m_bb.include( seg.end );
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

#include "core-Renderer\LineSegments.h"
#include "core-Renderer\Renderer.h"
#include "core\ResourcesManager.h"
#include "core\SingletonsManager.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( LineSegments, tls, AM_BINARY )
   PROPERTY( "segments", std::vector<LineSegment>, m_segments )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

LineSegments::LineSegments( const std::string& name )
: GeometryResource( name )
{
}

///////////////////////////////////////////////////////////////////////////////

void LineSegments::onResourceLoaded(ResourcesManager& mgr)
{
   Renderer& renderer = mgr.getInitializers().shared<Renderer>();
   renderer.implement<LineSegments> (*this);

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

void LineSegments::rebuild()
{
   impl().update(m_segments);
}

///////////////////////////////////////////////////////////////////////////////

void LineSegments::render()
{
   impl().render();
}

///////////////////////////////////////////////////////////////////////////////

BoundingVolume* LineSegments::calculateBoundingVolume() const
{
   return m_bb * m_identityMtx;
}

///////////////////////////////////////////////////////////////////////////////

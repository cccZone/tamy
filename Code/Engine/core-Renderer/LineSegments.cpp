#include "core-Renderer\LineSegments.h"
#include "core-Renderer\Renderer.h"
#include "core\ResourcesManager.h"
#include "core\SingletonsManager.h"
#include <math.h>

// TODO: TOP(1) !!!!! Save'owanie (przy zapisie swiata zapis resource'ow z resource manager'a).
// Innymi slowy - zintegrowac zapisywanie z resources manager'em

///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_RTTI(LineSegments)
   PARENT(RendererObject)
END_RTTI

///////////////////////////////////////////////////////////////////////////////

LineSegments::LineSegments()
{
}

///////////////////////////////////////////////////////////////////////////////

LineSegments::LineSegments(Filesystem& fs, 
                           const std::string& fileName)
{
   // TODO: wczytywanie z pliku
}

///////////////////////////////////////////////////////////////////////////////

void LineSegments::onLoaded(ResourcesManager& mgr)
{
   Renderer& renderer = mgr.getInitializers().shared<Renderer>();
   renderer.implement<LineSegments> (*this);
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

#include "core-Renderer\SpatialRepresentation.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\RenderState.h"
#include "core\BoundingVolume.h"
#include "core\AABoundingBox.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

SpatialRepresentation::SpatialRepresentation( Geometry& geometry )
   : m_geometry( geometry )
{
   // sort the states vector
   m_renderStates = geometry.getRenderStates();
   std::sort( m_renderStates.begin(), m_renderStates.end(), StateComparator() );
}

///////////////////////////////////////////////////////////////////////////////

SpatialRepresentation::~SpatialRepresentation()
{
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& SpatialRepresentation::getBoundingVolume() 
{
   return m_geometry.calculateBoundingVolume();
}

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\SpatialRepresentation.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\RenderState.h"
#include "core\BoundingVolume.h"
#include "core\AABoundingBox.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

SpatialRepresentation::SpatialRepresentation( Geometry& geometry )
   : m_geometry( geometry )
   , m_globalBoundingVolume( new AABoundingBox( D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( 0, 0, 0 ) ) )
{
   // sort the states vector
   m_renderStates = geometry.getRenderStates();
   std::sort( m_renderStates.begin(), m_renderStates.end(), StateComparator() );
}

///////////////////////////////////////////////////////////////////////////////

SpatialRepresentation::~SpatialRepresentation()
{
   delete m_globalBoundingVolume;
   m_globalBoundingVolume = NULL;
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& SpatialRepresentation::getBoundingVolume() 
{
   delete m_globalBoundingVolume;
   m_globalBoundingVolume = m_geometry.calculateBoundingVolume();

   return *m_globalBoundingVolume;
}

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\SpatialRepresentation.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\Geometry.h"
#include "core\BoundingVolume.h"
#include "core\AABoundingBox.h"


///////////////////////////////////////////////////////////////////////////////

SpatialRepresentation::SpatialRepresentation( Geometry& geometry )
   : m_geometry( geometry )
   , m_visibilityTag(-1)
   , m_globalBoundingVolume( new AABoundingBox( D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( 0, 0, 0 ) ) )
{
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
   if ( m_geometry.hasGeometry() )
   {
      delete m_globalBoundingVolume;
      m_globalBoundingVolume = m_geometry.calculateBoundingVolume();
   }

   return *m_globalBoundingVolume;
}

///////////////////////////////////////////////////////////////////////////////

void SpatialRepresentation::tagAsVisible( int tag )
{
   m_visibilityTag = tag;
}

///////////////////////////////////////////////////////////////////////////////

void SpatialRepresentation::setVisible( int tag )
{
   m_geometry.setVisible( m_visibilityTag == tag );
}

///////////////////////////////////////////////////////////////////////////////

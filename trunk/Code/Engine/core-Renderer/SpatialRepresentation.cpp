#include "core-Renderer\SpatialRepresentation.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\Geometry.h"
#include "core\BoundingVolume.h"
#include "core\AABoundingBox.h"


///////////////////////////////////////////////////////////////////////////////

SpatialRepresentation::SpatialRepresentation(Renderable& renderable)
: m_renderable(renderable)
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
   if ( m_renderable.hasGeometry() )
   {
      delete m_globalBoundingVolume;
      m_globalBoundingVolume = m_renderable.getGeometry().calculateBoundingVolume() * m_renderable.getGlobalMtx();
   }

   return *m_globalBoundingVolume;
}

///////////////////////////////////////////////////////////////////////////////

void SpatialRepresentation::tagAsVisible(int tag)
{
   m_visibilityTag = tag;
}

///////////////////////////////////////////////////////////////////////////////

void SpatialRepresentation::setVisible(int tag)
{
   m_renderable.setVisible(m_visibilityTag == tag);
}

///////////////////////////////////////////////////////////////////////////////

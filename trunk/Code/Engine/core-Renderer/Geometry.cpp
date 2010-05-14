#include "core-Renderer\Geometry.h"
#include "core-Renderer\GeometryResource.h"
#include "core\BoundingVolume.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT(Geometry, Entity)
   PROPERTY( "resource", GeometryResource*, m_resource )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Geometry::Geometry()
: m_resource(NULL)
, m_vol(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

Geometry::Geometry(GeometryResource& resource)
: m_resource(&resource)
{
   m_vol = m_resource->calculateBoundingVolume();
}

///////////////////////////////////////////////////////////////////////////////

Geometry::~Geometry()
{
   delete m_vol; m_vol = NULL;
   m_resource = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::render()
{
   m_resource->render();
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& Geometry::calculateBoundingVolume() const
{
   ASSERT(m_vol != NULL, "Geometry entity is not initialized");
   return *m_vol;
}

///////////////////////////////////////////////////////////////////////////////

void Geometry::onObjectLoaded()
{
   delete m_vol;
   m_vol = NULL;

   if ( m_resource )
   {
      m_vol = m_resource->calculateBoundingVolume();
   }
   ASSERT(m_vol != NULL, "Geometry entity is not initialized");
}

///////////////////////////////////////////////////////////////////////////////

std::string Geometry::getGeometryName() const
{
   if ( m_resource )
   {
      return m_resource->getResourceName();
   }
   else
   {
      return "<<uninitialized geometry>>";
   }
}

///////////////////////////////////////////////////////////////////////////////

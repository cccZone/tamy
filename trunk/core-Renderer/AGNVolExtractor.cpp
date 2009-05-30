#include "core-Renderer\AGNVolExtractor.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core\AABoundingBox.h"
#include "core\BoundingSphere.h"

///////////////////////////////////////////////////////////////////////////////

void AGNVolExtractor::operator()(AbstractGraphicalNode* elem, AABoundingBox& output) const
{
   const BoundingSphere& sphere = elem->getBoundingSphere();
   output.min.x = sphere.origin.x - sphere.radius;
   output.min.y = sphere.origin.y - sphere.radius;
   output.min.z = sphere.origin.z - sphere.radius;
   output.max.x = sphere.origin.x + sphere.radius;
   output.max.y = sphere.origin.y + sphere.radius;
   output.max.z = sphere.origin.z + sphere.radius;
}

///////////////////////////////////////////////////////////////////////////////

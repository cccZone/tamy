#include "core-Renderer\AGNVolExtractor.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core\AABoundingBox.h"
#include "core\BoundingSphere.h"

///////////////////////////////////////////////////////////////////////////////

const BoundingSphere& AGNVolExtractor::operator()(AbstractGraphicalNode* elem) const
{
   return elem->getBoundingSphere();
}

///////////////////////////////////////////////////////////////////////////////

#pragma once

/// @file   TamyEditor/GeometryManualEditor.h
/// @brief  A manual editor capable of changing geometry's transformation

#include "NodeManualEditor.h"


///////////////////////////////////////////////////////////////////////////////

class Geometry;

///////////////////////////////////////////////////////////////////////////////

/**
 * A manual editor capable of changing the transformation of a spatial entity
 * the geometry is a child to.
 */
class GeometryManualEditor : public NodeManualEditor
{
public:
   GeometryManualEditor( Geometry& node );
};

///////////////////////////////////////////////////////////////////////////////

/// @file   TamyEditor/QueryableDebugGeometry.h
/// @brief  a queryable representation of a geometrical entity
#pragma once

#include "QueryableEntity.h"


///////////////////////////////////////////////////////////////////////////////

class DebugGeometry;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A queryable representation of a debug geometry.
 */
class QueryableDebugGeometry : public QueryableEntity
{
private:
   DebugGeometry&            m_geometry;

public:
   /**
    * Constructor.
    *
    * @param geometry
    */
   QueryableDebugGeometry( DebugGeometry& geometry );

   // -------------------------------------------------------------------------
   // QueryableEntity implementation
   // -------------------------------------------------------------------------
   void render( Renderer& renderer, PixelShader& shader );
};

///////////////////////////////////////////////////////////////////////////////

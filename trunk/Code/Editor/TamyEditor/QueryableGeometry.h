#pragma once

/// @file   TamyEditor/QueryableGeometry.h
/// @brief  a queryable representation of a geometrical entity

#include "QueryableEntity.h"


///////////////////////////////////////////////////////////////////////////////

class Geometry;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A queryable representation of a geometrical entity.
 */
class QueryableGeometry : public QueryableEntity
{
private:
   Geometry&            m_geometry;

public:
   /**
    * Constructor.
    *
    * @param geometry
    */
   QueryableGeometry( Geometry& geometry );

   // -------------------------------------------------------------------------
   // QueryableEntity implementation
   // -------------------------------------------------------------------------
   void render( Renderer& renderer, PixelShader& shader );
};

///////////////////////////////////////////////////////////////////////////////

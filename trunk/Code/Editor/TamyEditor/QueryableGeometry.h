#pragma once

/// @file   TamyEditor/QueryableGeometry.h
/// @brief  a queryable representation of a geometrical entity

#include "QueryableEntity.h"


///////////////////////////////////////////////////////////////////////////////

class Geometry;

///////////////////////////////////////////////////////////////////////////////

/**
 * A queryable representation of a geometrical entity.
 */
class QueryableGeometry : public QueryableEntity
{
private:
   Geometry&            m_geometry;
   SceneQueryEffect*    m_effect;
   Attributes           m_attributes;

public:
   /**
    * Constructor.
    *
    * @param renderable
    */
   QueryableGeometry( Geometry& geometry );

   // -------------------------------------------------------------------------
   // QueryableEntity implementation
   // -------------------------------------------------------------------------
   void initialize( SceneQueryEffect& effect );

   // -------------------------------------------------------------------------
   // Attributed implementation
   // -------------------------------------------------------------------------
   const Attributes& getAttributes() const { return m_attributes; }
   void render();
};

///////////////////////////////////////////////////////////////////////////////

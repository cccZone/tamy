#pragma once

/// @file   TamyEditor/QueryableRenderable.h
/// @brief  a queryable representation of a renderable entity

#include "QueryableEntity.h"


///////////////////////////////////////////////////////////////////////////////

class Renderable;

///////////////////////////////////////////////////////////////////////////////

/**
 * A queryable representation of a renderable entity.
 */
class QueryableRenderable : public QueryableEntity
{
private:
   Renderable&          m_renderable;
   SceneQueryEffect*    m_effect;
   Attributes           m_attributes;
public:
   /**
    * Constructor.
    *
    * @param renderable
    */
   QueryableRenderable( Renderable& renderable );

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

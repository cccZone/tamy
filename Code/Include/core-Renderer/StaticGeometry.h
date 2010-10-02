#pragma once

/// @file   core-Renderer\StaticGeometry.h
/// @brief  A static geometry entity

#include "core-Renderer\Geometry.h"


///////////////////////////////////////////////////////////////////////////////

class VertexShader;
class SpatialEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * A static geometry entity.
 */
class StaticGeometry : public Geometry
{
   DECLARE_CLASS( StaticGeometry )

private:
   VertexShader*              m_vertexShader;

public:
   /**
    * Default constructor.
    */
   StaticGeometry();

   /**
    * Constructor.
    *
    * @param resource   shared geometry resource
    */
   StaticGeometry( GeometryResource& resource );
   ~StaticGeometry();

protected:
   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void onComponentAdded( Component< Model >& component );

   // -------------------------------------------------------------------------
   // Geometry implementation
   // -------------------------------------------------------------------------
   void onPreRender();
   void onPostRender();
};

///////////////////////////////////////////////////////////////////////////////

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
   DECLARE_CLASS()

private:
   VertexShader*              m_vertexShader;

public:
   /**
    * Default constructor.
    *
    * @param name
    */
   StaticGeometry( const char* name = NULL );

   /**
    * Constructor.
    *
    * @param resource   shared geometry resource
    * @param name
    */
   StaticGeometry( GeometryResource& resource, const char* name = NULL );
   ~StaticGeometry();

protected:
   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   Entity* cloneSelf() const;

   // -------------------------------------------------------------------------
   // Geometry implementation
   // -------------------------------------------------------------------------
   RCBindVertexShader* onPreRender( Renderer& renderer );
   void onPostRender( Renderer& renderer );

private:
   void initialize();
};

///////////////////////////////////////////////////////////////////////////////

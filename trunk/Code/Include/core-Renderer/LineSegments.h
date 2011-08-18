#ifndef _LINE_SEGMENTS_H
#define _LINE_SEGMENTS_H

/// @file   core-Renderer\LineSegments.h
/// @brief  geometry of a mesh composed of line segments

#include <vector>
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\RenderCommand.h"
#include "core-Renderer\RenderResource.h"
#include "core\AABoundingBox.h"
#include "core\Color.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A helper structure describing a line segment extended between two points
 * in 3D space.
 */
struct LineSegment
{
   D3DXVECTOR3 start;
   D3DXVECTOR3 end;
   Color       color;

   /**
    * Constructor.
    */
   inline LineSegment();
   /**
    * Constructor.
    *
    * @param start   segment start pos
    * @param end     segment end pos
    * @param color   segment color
    */
   inline LineSegment( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color = Color() );

   inline bool operator==( const LineSegment& rhs ) const;
   inline bool operator!=( const LineSegment& rhs ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This is an abstract representation of a mesh geometry composed of
 * line segments.
 */
class LineSegments : public GeometryResource, public RenderResource
{
   DECLARE_RESOURCE( LineSegments )

private:
   std::vector<LineSegment>   m_segments;

   D3DXMATRIX                 m_identityMtx;
   AABoundingBox              m_bb;

public:
   /**
    * Constructor.
    */
   LineSegments( const std::string& name = "" );

   /**
    * Adds a new line segment to the geometry.
    */
   void add(const LineSegment& segment);

   /**
    * Removes a line from the geometry.
    */
   void remove(const LineSegment& segment);

   /**
    * Removes all lines.
    */
   void clear();

   /**
    * Returns the line segments of this geometry.
    */
   inline const std::vector< LineSegment >& getSegments() const { return m_segments; }

   // -------------------------------------------------------------------------
   // GeometryResource implementation
   // -------------------------------------------------------------------------
   inline const BoundingVolume& getBoundingVolume() const { return m_bb; }
   void render( Renderer& renderer );

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded( ResourcesManager& mgr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Line segments rendering command.
 */
class RCRenderLineSegments : public RenderCommand
{
private:
   LineSegments&        m_segments;

public:
   RCRenderLineSegments( LineSegments& segments ) : m_segments( segments ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\LineSegments.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _LINE_SEGMENTS_H

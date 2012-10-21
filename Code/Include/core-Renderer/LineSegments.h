/// @file   core-Renderer\LineSegments.h
/// @brief  geometry of a mesh composed of line segments
#ifndef _LINE_SEGMENTS_H
#define _LINE_SEGMENTS_H

#include <vector>
#include "core\MemoryRouter.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\RenderCommand.h"
#include "core-Renderer\RenderResource.h"
#include "core\AABoundingBox.h"
#include "core\Color.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A helper structure describing a line segment extended between two points
 * in 3D space.
 */
struct LineSegment
{
   DECLARE_ALLOCATOR( LineSegment, AM_ALIGNED_16 );

   Vector      start;
   Vector      end;
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
   inline LineSegment( const Vector& start, const Vector& end, const Color& color = Color() );

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
   DECLARE_ALLOCATOR( LineSegments, AM_ALIGNED_16 );
   DECLARE_RESOURCE();

private:
   Array<LineSegment>         m_segments;

   bool                       m_boundsDirty;
   AABoundingBox              m_bb;

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   LineSegments( const FilePath& resourceName = FilePath() );

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
   inline const Array< LineSegment >& getSegments() const { return m_segments; }

   // -------------------------------------------------------------------------
   // GeometryResource implementation
   // -------------------------------------------------------------------------
   const BoundingVolume& getBoundingVolume();
   void render( Renderer& renderer );

private:
   /**
    * Recalculates the bounding volume.
    */
   void calculateBoundingVolume();

};

///////////////////////////////////////////////////////////////////////////////

/**
 * Line segments rendering command.
 */
class RCRenderLineSegments : public RenderCommand
{
   DECLARE_ALLOCATOR( RCRenderLineSegments, AM_DEFAULT );

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

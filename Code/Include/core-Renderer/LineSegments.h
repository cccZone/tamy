#ifndef _LINE_SEGMENTS_H
#define _LINE_SEGMENTS_H

/// @file   core-Renderer\LineSegments.h
/// @brief  geometry of a mesh composed of line segments

#include <vector>
#include "core-Renderer\GeometryResource.h"
#include "core\Resource.h"
#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"
#include "core\AABoundingBox.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A helper structure describing a line segment extended between two points
 * in 3D space.
 */
struct LineSegment
{
   D3DXVECTOR3 start;
   D3DXVECTOR3 end;

   /**
    * Constructor.
    */
   inline LineSegment();
   inline LineSegment(const D3DXVECTOR3& start, const D3DXVECTOR3& end);

   inline bool operator==(const LineSegment& rhs) const;
   inline bool operator!=(const LineSegment& rhs) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Graphics library dependent implementation of geometry composed of lines.
 */
class LineSegmentsImpl : public RendererObjectImpl
{
public:
   virtual ~LineSegmentsImpl() {}

   /**
    * Renders the mesh using the library calls.
    */
   virtual void render() {}

   /**
    * Updates the buffer containing the lines.
    */
   virtual void update(const std::vector<LineSegment>& segments) {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This is an abstract representation of a mesh geometry composed of
 * line segments.
 */
class LineSegments : public GeometryResource, 
                     public Resource,
                     public TRendererObject<LineSegmentsImpl>
{
   DECLARE_RESOURCE( LineSegments )

private:
   std::vector<LineSegment> m_segments;

   D3DXMATRIX m_identityMtx;
   AABoundingBox m_bb;

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
    * Rebuilds the geometry stored on the implementation side.
    * Without running this method, you won't notice any changes 
    * you've made with add & remove methods.
    */
   void rebuild();

   // -------------------------------------------------------------------------
   // Geometry implementation
   // -------------------------------------------------------------------------
   void render();

   BoundingVolume* calculateBoundingVolume() const;

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded(ResourcesManager& mgr);
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\LineSegments.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _LINE_SEGMENTS_H

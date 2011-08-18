#pragma once

/// @file   core-Renderer\SkinnedGeometry.h
/// @brief  skin geometry

#include "core-Renderer\Geometry.h"
#include "core-Renderer\LitVertex.h"
#include "core\Array.h"
#include <d3dx9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class BoundingVolume;
class GeometryResource;
class Skeleton;
class SpatialEntity;
class Camera;
class VertexShader;
class Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * An entity that renders a skinned geometry
 */
class SkinnedGeometry : public Geometry
{
   DECLARE_CLASS( SkinnedGeometry )

private:
   Skeleton*                           m_skeleton;
   VertexShader*                       m_vertexShader;

   // runtime data
   BoundingVolume*                     m_vol;
   std::vector< Node* >                m_bones;
   Array< D3DXMATRIX >                 m_boneMatrices;

public:
   /**
    * Default constructor.
    */
   SkinnedGeometry();

   /**
    * Constructor.
    *
    * @param geometry         shared geometry resource
    * @param skeleton         skeleton resource that will move the skin
    */
   SkinnedGeometry( GeometryResource& geometry, Skeleton& skeleton );
   ~SkinnedGeometry();

protected:
   // -------------------------------------------------------------------------
   // Geometry implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer );
   void onPostRender( Renderer& renderer );

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void onAttached(Entity& parent);
   void onDetached(Entity& parent);
   void onComponentAdded( Component< Model >& component );

private:
   void instantiateSkeleton( Entity& parent );
};

///////////////////////////////////////////////////////////////////////////////

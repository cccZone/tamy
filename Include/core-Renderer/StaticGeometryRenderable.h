#pragma once

/// @file   core-Renderer\StaticGeometryRenderable.h
/// @brief  an entity with renderable static geometry that shares 
///         a material

#include "core\AABoundingBox.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\Face.h"
#include "core-Renderer\MeshTriangle.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

struct Triangle;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a static geometry object that can be rendered.
 */
class StaticGeometryRenderable : public Renderable
{
private:
   Material& m_material;
   std::vector<LitVertex> m_vertices;
   std::vector<Face > m_faces;
   AABoundingBox m_boundingVol;
   Array<D3DXMATRIX> m_renderingMtcs;
   D3DXMATRIX m_situation;

   TriangleSplitter<LitVertex, MeshTriangle> m_splitter;

public:
   /**
    * Constructor.
    * 
    * @param material      material this geometry will use
    * @param vertices      vertices the renderable is composed of
    * @param faces         triangles definitions
    */
   StaticGeometryRenderable(Material& material,
                            const std::vector<LitVertex>& vertices,
                            const std::vector<Face >& faces);

   virtual ~StaticGeometryRenderable();

   /**
    * The method returns a collection of vertices the object is composed of.
    *
    * @return     vector of vertices
    */
   const std::vector<LitVertex>& getVertices() const;

   /**
    * The method returns a collection of triangles the object is composed of.
    *
    * @return     vector of face definitions
    */
   const std::vector<Face >& getFaces() const;

   // -------------------------------------------------------------------------
   // Renderable implementation
   // -------------------------------------------------------------------------
   virtual void render() = 0;

   const BoundingVolume& getVolume();

   const Array<D3DXMATRIX>& getRenderingMatrices();

   const Material& getMaterial() const;

   const D3DXMATRIX& getGlobalTransform();

   // -------------------------------------------------------------------------
   // Interface required by StaticGeometryOctree
   // -------------------------------------------------------------------------
   void split(const D3DXPLANE& splitPlane, 
              StaticGeometryRenderable** frontSplit,
              StaticGeometryRenderable** backSplit);

   const BoundingVolume& getBoundingVolume();

protected:
   /**
    * Factory method for creating a solid implementation of this class.
    *
    * @param material      material this geometry will use
    * @param vertices      vertices the renderable is composed of
    * @param faces         triangles definitions
    * @return              new StaticGeometryRenderable instance
    */
   virtual StaticGeometryRenderable* createSelf(Material& material,
                                                const std::vector<LitVertex>& vertices,
                                                const std::vector<Face >& faces) = 0;

private:
   void createNewObject(const Array<MeshTriangle*>& triangles,
                        std::vector<LitVertex>& outVertices,
                        std::vector<Face >& outFaces);
};

///////////////////////////////////////////////////////////////////////////////


#pragma once

/// @file   core-Renderer\Skeleton.h
/// @brief  a resource with a skeletal structure of a model

#include <string>
#include "core\Resource.h"
#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"
#include "core-Renderer\LitVertex.h"
#include <d3dx9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * Graphics library dependent implementation of the skin vertex weights stream.
 */
class SkeletonImpl : public RendererObjectImpl
{
public:
   virtual ~SkeletonImpl() {}

   /**
    * Sets the representation in the graphics device vertex stream.
    */
   virtual void setInStream() {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A skeleton represents a hierarchy of nodes along with
 * a set of animations that operate on it
 */
class Skeleton : public Resource, public TRendererObject< SkeletonImpl >
{
   DECLARE_RESOURCE( Skeleton )

private:
   D3DXMATRIX                    m_bindShapeMtx;
   std::vector< std::string >    m_boneNames;
   std::vector< D3DXMATRIX >     m_invBoneMatrices;
   std::vector< VertexWeight >   m_weights;

   D3DXMATRIX                    m_identityMtx;

public:  
   /**
    * Constructor.
    */
   Skeleton( const std::string& path = "" );
   ~Skeleton();

   /**
    * Sets a new shape bind matrix.
    *
    * @param bindShapeMtx
    */
   void setShapeBindMatrix( const D3DXMATRIX& bindShapeMtx );

   /**
    * Sets a transformation definition for the specified bone.
    *
    * @param boneName      name of the bone
    * @param invBoneMtx    inverted transformation matrix of the bone
    */
   void setTransformation( const std::string& boneName, const D3DXMATRIX& invBoneMtx );

   /**
    * Adds a new vertex bone weight definition.
    *
    * @param vertexIdx
    * @param boneId
    * @param weight
    */
   void addWeight( unsigned int vertexIdx, const std::string& boneId, float weight );

   /**
    * Returns the index assigned to the specified bone.
    * 
    * @param boneId
    */
   int getBoneIndex( const std::string& boneId ) const;

   /**
    * Returns the number of bones defined.
    */
   inline unsigned int getBonesCount() const { return m_boneNames.size(); }

   /**
    * Returns a name of the specified bone.
    *
    * @param boneIdx       index of the bone in the skeleton
    */
   inline const std::string getBoneName( unsigned int boneIdx ) const { return m_boneNames[boneIdx]; }

   /**
    * Returns an inverted bind pose matrix assigned to the specified bone.
    *
    * @param boneName      name of the bone
    */
   const D3DXMATRIX& getInvBindPoseMtx( const std::string& boneName ) const;

   /**
    * Returns the weights assigned to a mesh vertices.
    */
   inline const std::vector< VertexWeight >& getVertexWeights() const { return m_weights; }

   /**
    * Sets the data in the rendering stream. Call this before the mesh gets rendered.
    */
   void setInStream();

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onComponentAdded( Component< ResourcesManager >& component );
   void onComponentRemoved( Component< ResourcesManager >& component );
};

///////////////////////////////////////////////////////////////////////////////

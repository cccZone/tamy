/// @file   core-Renderer\Skeleton.h
/// @brief  a resource with a skeletal structure of a model
#pragma once

#include <string>
#include "core\Resource.h"
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\RenderCommand.h"
#include "core-Renderer\RenderResource.h"
#include "core\Matrix.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class SpatialEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * A skeleton represents a hierarchy of nodes along with
 * a set of animations that operate on it
 */
class Skeleton : public Resource, public RenderResource
{
   DECLARE_ALLOCATOR( Skeleton, AM_ALIGNED_16 );
   DECLARE_RESOURCE()

private:
   Matrix                        m_bindShapeMtx;
   std::vector< std::string >    m_boneNames;
   Array< Matrix >               m_invBoneMatrices;
   std::vector< VertexWeight >   m_weights;

public:  
   /**
    * Constructor.
    *
    * @param resourceName
    */
   Skeleton( const FilePath& resourceName = FilePath() );
   ~Skeleton();

   /**
    * Sets a new shape bind matrix.
    *
    * @param bindShapeMtx
    */
   void setShapeBindMatrix( const Matrix& bindShapeMtx );

   /**
    * Sets a transformation definition for the specified bone.
    *
    * @param boneName      name of the bone
    * @param invBoneMtx    inverted transformation matrix of the bone
    */
   void setTransformation( const std::string& boneName, const Matrix& invBoneMtx );

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
   const Matrix& getInvBindPoseMtx( const std::string& boneName ) const;

   /**
    * Returns the weights assigned to a mesh vertices.
    */
   inline const std::vector< VertexWeight >& getVertexWeights() const { return m_weights; }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Render command that binds a skeleton to a render device.
 */
class RCBindSkeleton : public RenderCommand
{
   DECLARE_ALLOCATOR( RCBindSkeleton, AM_DEFAULT );

private:
   Skeleton&         m_skeleton;

public:
   RCBindSkeleton( Skeleton& skeleton ) : m_skeleton( skeleton ) {}

   // -------------------------------------------------------------------------
   // RenderCommand
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Render command that unbinds a skeleton from a render device.
 */
class RCUnbindSkeleton : public RenderCommand
{
   DECLARE_ALLOCATOR( RCUnbindSkeleton, AM_DEFAULT );

private:
   Skeleton&         m_skeleton;

public:
   RCUnbindSkeleton( Skeleton& skeleton ) : m_skeleton( skeleton ) {}

   // -------------------------------------------------------------------------
   // RenderCommand
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

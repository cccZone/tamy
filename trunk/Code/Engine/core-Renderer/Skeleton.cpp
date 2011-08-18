#include "core-Renderer\Skeleton.h"
#include "core-Renderer\Renderer.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( Skeleton, Resource, tsk, AM_BINARY )
   PROPERTY( D3DXMATRIX, m_bindShapeMtx )
   PROPERTY( std::vector< std::string >, m_boneNames )
   PROPERTY( std::vector< D3DXMATRIX >, m_invBoneMatrices )
   PROPERTY( std::vector< VertexWeight >, m_weights )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

Skeleton::Skeleton( const std::string& path )
   : Resource( path )
{
   D3DXMatrixIdentity( &m_identityMtx );
   D3DXMatrixIdentity( &m_bindShapeMtx );
}

///////////////////////////////////////////////////////////////////////////////

Skeleton::~Skeleton()
{
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::setShapeBindMatrix( const D3DXMATRIX& bindShapeMtx )
{
   D3DXMATRIX prevInvMtx;
   D3DXMatrixInverse( &prevInvMtx, NULL, &m_bindShapeMtx );

   m_bindShapeMtx = bindShapeMtx;

   // transform the matrices to the new bind shape mtx
   unsigned int count = m_invBoneMatrices.size();
   for( unsigned int i = 0; i < count; ++i )
   {
      m_invBoneMatrices[i] = m_bindShapeMtx * m_invBoneMatrices[i] * prevInvMtx;
   }
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::setTransformation( const std::string& boneName, const D3DXMATRIX& invBoneMtx )
{
   // first check if we're not trying to replace an existing definition
   unsigned int count = m_boneNames.size();
   unsigned int i;
   for( i = 0; i < count; ++i )
   {
      if ( m_boneNames[i] == boneName )
      {
         break;
      }
   }

   if ( i >= count )
   {
      // this is a new bone
      m_boneNames.push_back( boneName );
      m_invBoneMatrices.push_back( m_bindShapeMtx * invBoneMtx );
   }
   else
   {
      // we're replacing an existing bone definition
      m_invBoneMatrices[i] = m_bindShapeMtx * invBoneMtx;
   }
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::addWeight( unsigned int vertexIdx, const std::string& boneId, float weight )
{
   // make sure the vertex we're trying to describe is in our array
   while ( vertexIdx >= m_weights.size() )
   {
      m_weights.push_back( VertexWeight() );
   }

   // find the index assigned to the specified bone
   int boneIdx = getBoneIndex( boneId );
   if ( boneIdx < 0 )
   {
      boneIdx = m_boneNames.size();
      m_boneNames.push_back( boneId );
   }

   // find the next index to fill in
   unsigned int nextIdx = 0;
   for ( nextIdx = 0; nextIdx < 4; ++nextIdx )
   {
      if ( m_weights[ vertexIdx ].m_indices[ nextIdx ] < 0 )
      {
         break;
      }
   }
   ASSERT_MSG( nextIdx < 4, "Too many skin weights assigned" );
   m_weights[ vertexIdx ].m_weights[ nextIdx ] = weight;
   m_weights[ vertexIdx ].m_indices[ nextIdx ] = (float)boneIdx;
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& Skeleton::getInvBindPoseMtx( const std::string& boneName ) const
{
   int idx = getBoneIndex( boneName );
   if ( idx < 0 )
   {
      return m_identityMtx;
   }
   else
   {
      return m_invBoneMatrices[ idx ];
   }
}

///////////////////////////////////////////////////////////////////////////////

int Skeleton::getBoneIndex( const std::string& boneId ) const
{
   unsigned int count = m_boneNames.size();
   int idx = 0;
   for ( idx = 0; idx < (int)count; ++idx )
   {
      if ( m_boneNames[idx] == boneId )
      {
         return idx;
      }
   }
   return -1;
}

///////////////////////////////////////////////////////////////////////////////

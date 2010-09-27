#include "core-Renderer\SkinnedGeometry.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\Skeleton.h"
#include "core-Renderer\SpatialEntity.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Shader.h"
#include "core-MVC.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( SkinnedGeometry, Geometry )
   PROPERTY_EDIT( "skeleton", Skeleton*, m_skeleton )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

SkinnedGeometry::SkinnedGeometry()
   : Geometry()
   , m_skeleton( NULL )
   , m_vol( new AABoundingBox() )
   , m_vertexShader( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

SkinnedGeometry::SkinnedGeometry( GeometryResource& geometry, Skeleton& skeleton )
   : Geometry( geometry )
   , m_skeleton( &skeleton )
   , m_vertexShader( NULL )
   , m_vol( new AABoundingBox() )
{
}

///////////////////////////////////////////////////////////////////////////////

SkinnedGeometry::~SkinnedGeometry()
{
   delete m_vol; m_vol = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SkinnedGeometry::onPreRender()
{
   if ( !m_skeleton || !m_vertexShader || m_bones.empty() )
   {
      return;
   }

   // set the transformation matrices
   unsigned int bonesCount = m_bones.size();
   for ( unsigned int i = 0; i < bonesCount; ++i )
   {
      Node* bone = m_bones[i];
      D3DXMATRIX& boneMatrix = m_boneMatrices[i];
      const D3DXMATRIX& invBindPoseMtx = m_skeleton->getInvBindPoseMtx( bone->getName() );

      boneMatrix = invBindPoseMtx * bone->getGlobalMtx() * m_camera->getViewMtx();
   }
   m_vertexShader->setMtxArray( "g_mSkinningMatrices", m_boneMatrices, m_boneMatrices.size() );
   m_vertexShader->setMtx( "g_mProjection", m_camera->getProjectionMtx() );

   // set the stream with blend weights
   m_skeleton->setInStream();

   m_vertexShader->beginRendering();
}

///////////////////////////////////////////////////////////////////////////////

void SkinnedGeometry::onPostRender()
{
   if ( m_vertexShader )
   {
      m_vertexShader->endRendering();
   }
}

///////////////////////////////////////////////////////////////////////////////

void SkinnedGeometry::instantiateSkeleton( Entity& parent )
{
   ASSERT( m_bones.empty() );
   m_bones.clear();

   SpatialEntity* parentNode = dynamic_cast<SpatialEntity* > ( &parent );
   if ( !m_skeleton || !parentNode )
   {
      return;
   }

   unsigned int bonesCount = m_skeleton->getBonesCount();

   D3DXMATRIX identityMtx;
   D3DXMatrixIdentity( &identityMtx );

   m_bones.resize( bonesCount );
   m_boneMatrices.allocate( bonesCount );
   for ( unsigned int boneIdx = 0; boneIdx < bonesCount; ++boneIdx )
   {
      m_boneMatrices.push_back( identityMtx );
   }

   for ( unsigned int boneIdx = 0; boneIdx < bonesCount; ++boneIdx )
   {
      const std::string& boneId = m_skeleton->getBoneName( boneIdx );
      Node* bone = parentNode->findNode( boneId );

      // verify that we get the bone - we either address the entire skeleton, or none of it
      ASSERT( bone != NULL, "Bone '%s' does not exist", boneId.c_str() );
      if ( !bone )
      {
         m_bones.clear();
         return;
      }

      m_bones[ boneIdx ] = bone;
   }
}

///////////////////////////////////////////////////////////////////////////////

void SkinnedGeometry::onAttached( Entity& parent )
{
   __super::onAttached( parent );
   instantiateSkeleton( parent );
}

///////////////////////////////////////////////////////////////////////////////

void SkinnedGeometry::onDetached( Entity& parent )
{
   m_bones.clear();
   m_boneMatrices.clear();
   __super::onDetached( parent );
}

///////////////////////////////////////////////////////////////////////////////

void SkinnedGeometry::onComponentAdded( Component< Model >& component )
{
   __super::onComponentAdded( component );

   ModelComponent< ResourcesManager >* comp = dynamic_cast< ModelComponent< ResourcesManager >* >( &component );
   if ( comp )
   {
      // load the shader
      ResourcesManager& rm = comp->get();
      static const char* shaderName = "Renderer/Shaders/SkinnedGeometry.vsh";
      m_vertexShader = dynamic_cast< Shader* >( rm.findResource( shaderName ) );
      if ( !m_vertexShader )
      {
         m_vertexShader = new Shader( shaderName, SHT_VERTEX_SHADER );
         m_vertexShader->setVertexDescription( VDI_SIMPLE_SKINNING );
         rm.addResource( m_vertexShader );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

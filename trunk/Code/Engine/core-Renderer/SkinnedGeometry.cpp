#include "core-Renderer\SkinnedGeometry.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\Skeleton.h"
#include "core-MVC\SpatialEntity.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Defines.h"
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

bool SkinnedGeometry::onPreRender( Renderer& renderer )
{
   if ( !m_skeleton || !m_vertexShader || m_bones.empty() )
   {
      return false;
   }

   Camera& camera = renderer.getActiveCamera();

   new ( renderer() ) RCBindSkeleton( *m_skeleton );
   RCBindVertexShader* comm = new ( renderer() ) RCBindVertexShader( *m_vertexShader );

   // set the transformation matrices
   unsigned int bonesCount = m_bones.size();
   for ( unsigned int i = 0; i < bonesCount; ++i )
   {
      Node* bone = m_bones[i];
      D3DXMATRIX& boneMatrix = m_boneMatrices[i];
      const D3DXMATRIX& invBindPoseMtx = m_skeleton->getInvBindPoseMtx( bone->getName() );

      boneMatrix = invBindPoseMtx * bone->getGlobalMtx() * camera.getViewMtx();
   }
   comm->setMtx( "g_mSkinningMatrices", m_boneMatrices, m_boneMatrices.size() );
   comm->setMtx( "g_mProjection", camera.getProjectionMtx() );

   return true;
}

///////////////////////////////////////////////////////////////////////////////

void SkinnedGeometry::onPostRender( Renderer& renderer )
{
   if ( !m_skeleton || !m_vertexShader || m_bones.empty() )
   {
      return;
   }

   new ( renderer() ) RCUnbindVertexShader( *m_vertexShader );
   new ( renderer() ) RCUnbindSkeleton( *m_skeleton );
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
      static char tmpStr[256];
      sprintf( tmpStr, "Bone '%s' does not exist", boneId.c_str() );
      ASSERT_MSG( bone != NULL, tmpStr );
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
      static FilePath shaderName( SHADERS_DIR "SkinnedGeometry.vsh" );
      m_vertexShader = rm.findResource< VertexShader >( shaderName );
      if ( !m_vertexShader )
      {
         m_vertexShader = new VertexShader( shaderName );
         m_vertexShader->setVertexDescription( VDI_SIMPLE_SKINNING );

         // load the shader code
         File* shaderFile = rm.getFilesystem().open( shaderName, std::ios_base::in );
         StreamBuffer< char > shaderCodeBuf( *shaderFile );
         m_vertexShader->setScript( shaderCodeBuf.getBuffer() );
         delete shaderFile;

         rm.addResource( m_vertexShader );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

Entity* SkinnedGeometry::cloneSelf() const
{
   SkinnedGeometry* entity = new SkinnedGeometry( *this );
   return entity;
}

///////////////////////////////////////////////////////////////////////////////

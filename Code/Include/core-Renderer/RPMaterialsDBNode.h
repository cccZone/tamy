/// @file   core-Renderer/RPMaterialsDBNode.h
/// @brief  a rendering pipeline node that runs the materials DB and creates a materials atlas
#pragma once

#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RPTextureOutput;
class MaterialsDB;
class PixelShader;
class RenderTarget;
class DepthBuffer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node that runs the materials DB and creates a materials atlas.
 */
class RPMaterialsDBNode : public RenderingPipelineNode
{
   DECLARE_ALLOCATOR( RPMaterialsDBNode, AM_ALIGNED_16 );
   DECLARE_CLASS()

private:
   MaterialsDB&                     m_materialsDB;

   PixelShader*                     m_descriptorsShader;
   PixelShader*                     m_atlasShader;

   RPTextureOutput*                 m_descriptorsOutput;
   RPTextureOutput*                 m_texturesAtlas;

   TRuntimeVar< int >               m_dbTimestamp;
   TRuntimeVar< RenderTarget* >     m_descriptorsRT;
   TRuntimeVar< DepthBuffer* >      m_descriptorsDB;
   TRuntimeVar< RenderTarget* >     m_texturesAtlasRT;
   TRuntimeVar< DepthBuffer* >      m_texturesAtlasDB;

public:
   /**
    * Constructor.
    */
   RPMaterialsDBNode();
   ~RPMaterialsDBNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreatePrerequisites( RenderingPipelineTransaction& transaction ) const;
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

private:
   void renderMaterialDescriptors( Renderer& renderer, RenderTarget* rt, DepthBuffer* depthBuf,const Array< Vector >& materialsData, const Array< Vector >& textureCoords ) const;
   void renderTextureAtlas( Renderer& renderer, RenderTarget* rt, DepthBuffer* depthBuf,const Array< Vector >& textureCoords ) const;
};

///////////////////////////////////////////////////////////////////////////////

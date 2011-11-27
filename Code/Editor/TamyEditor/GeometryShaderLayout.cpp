#include "GeometryShaderLayout.h"
#include "core.h"
#include "core-Renderer.h"

// blocks & sockets
#include "GeometryShaderSocketRepresentation.h"
#include "GBVertexShader.h"
#include "GBSpatialEntity.h"
#include "GBCamera.h"
#include "GBTexture.h"
#include "GBFloat.h"
#include "GBVec4.h"
#include "GBBool.h"
#include "GBTimer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( GeometryShaderLayout, Resource, rgsl, AM_BINARY );
   PROPERTY( GeometryShader*, m_pipeline );
   PROPERTY( std::vector< GraphBlock* >, m_blocks );
   PROPERTY( std::vector< GraphBlockConnection* >, m_connections );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

GeometryShaderLayout::GeometryShaderLayout( const FilePath& resourceName )
   : PipelineLayout< GeometryShader, GeometryShaderNode >( resourceName )
{
   // create block-to-node associations
   initBlocksFactory();

   // customize the looks
   setBackgroundBrush( QColor( 209, 226, 165 ) );
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderLayout::initBlocksFactory()
{
   associate< GNVertexShader, GBVertexShader >();
   associate< GNSpatialEntity, GBSpatialEntity >();
   associate< GNCamera, GBCamera >();
   associate< GNTexture, GBTexture >();
   associate< GNFloat, GBFloat >();
   associate< GNVec4, GBVec4 >();
   associate< GNBool, GBBool >();
   associate< GNTimer, GBTimer >();
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderLayout::initSocketsFactory( PipelineBlock& block )
{
   GenericFactory< GBNodeSocket, GraphBlockSocket >& factory = block.getSocketsFactory();
   factory.associate< GSTextureInput, GSSTexture >();
   factory.associate< GSTextureOutput, GSSTexture >();
   factory.associate< GSFloatInput, GSSFloat >();
   factory.associate< GSFloatOutput, GSSFloat >();
   factory.associate< GSVec4Input, GSSVec4 >();
   factory.associate< GSVec4Output, GSSVec4 >();
   factory.associate< GSMatrixInput, GSSMatrix >();
   factory.associate< GSMatrixOutput, GSSMatrix >();
   factory.associate< GSBoolInput, GSSBool >();
   factory.associate< GSBoolOutput, GSSBool >();
}

///////////////////////////////////////////////////////////////////////////////

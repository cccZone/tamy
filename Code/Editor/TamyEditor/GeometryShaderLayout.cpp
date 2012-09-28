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

BEGIN_RESOURCE( GeometryShaderLayout, rgsl, AM_BINARY );
   PROPERTY( GeometryShader*, m_pipeline );
   PROPERTY( std::vector< GraphBlock* >, m_blocks );
   PROPERTY( std::vector< GraphBlockConnection* >, m_connections );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

GeometryShaderLayout::GeometryShaderLayout( const FilePath& resourceName )
   : TGraphLayout< GeometryShader, GeometryShaderNode >( resourceName )
{
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderLayout::initBlocksFactory( BlocksFactory& factory )
{
   factory.associate< GNVertexShader, GBVertexShader >();
   factory.associate< GNSpatialEntity, GBSpatialEntity >();
   factory.associate< GNCamera, GBCamera >();
   factory.associate< GNTexture, GBTexture >();
   factory.associate< GNFloat, GBFloat >();
   factory.associate< GNVec4, GBVec4 >();
   factory.associate< GNBool, GBBool >();
   factory.associate< GNTimer, GBTimer >();
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderLayout::initSocketsFactory( SocketsFactory& factory )
{
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

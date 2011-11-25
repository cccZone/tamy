#include "MaterialLayout.h"
#include "core.h"
#include "core-Renderer.h"

// blocks & sockets
#include "MaterialSocketRepresentation.h"
#include "MBPixelShader.h"
#include "MBSpatialEntity.h"
#include "MBCamera.h"
#include "MBTexture.h"
#include "MBFloat.h"
#include "MBVec4.h"
#include "MBBool.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( MaterialLayout, Resource, rml, AM_BINARY );
   PROPERTY( Material*, m_pipeline );
   PROPERTY( std::vector< GraphBlock* >, m_blocks );
   PROPERTY( std::vector< GraphBlockConnection* >, m_connections );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

MaterialLayout::MaterialLayout( const FilePath& resourceName )
   : PipelineLayout< Material, MaterialNode >( resourceName )
{
   // create block-to-node associations
   initBlocksFactory();

   // customize the looks
   setBackgroundBrush( QColor( 209, 226, 165 ) );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialLayout::initBlocksFactory()
{
   associate< MNPixelShader, MBPixelShader >();
   associate< MNSpatialEntity, MBSpatialEntity >();
   associate< MNCamera, MBCamera >();
   associate< MNTexture, MBTexture >();
   associate< MNFloat, MBFloat >();
   associate< MNVec4, MBVec4 >();
   associate< MNBool, MBBool >();
}

///////////////////////////////////////////////////////////////////////////////

void MaterialLayout::initSocketsFactory( PipelineBlock& block )
{
   GenericFactory< GBNodeSocket, GraphBlockSocket >& factory = block.getSocketsFactory();
   factory.associate< MSTextureInput, MSSTexture >();
   factory.associate< MSTextureOutput, MSSTexture >();
   factory.associate< MSFloatInput, MSSFloat >();
   factory.associate< MSFloatOutput, MSSFloat >();
   factory.associate< MSVec4Input, MSSVec4 >();
   factory.associate< MSVec4Output, MSSVec4 >();
   factory.associate< MSMatrixInput, MSSMatrix >();
   factory.associate< MSMatrixOutput, MSSMatrix >();
   factory.associate< MSBoolInput, MSSBool >();
   factory.associate< MSBoolOutput, MSSBool >();
}

///////////////////////////////////////////////////////////////////////////////

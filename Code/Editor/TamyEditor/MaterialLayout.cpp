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
#include "MBInstanceTexture.h"
#include "MBSurfaceProperties.h"
#include "MBTimer.h"
#include "MBMaterialIndex.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( MaterialLayout, rml, AM_BINARY );
   PROPERTY( Material*, m_pipeline );
   PROPERTY( std::vector< GraphBlock* >, m_blocks );
   PROPERTY( std::vector< GraphBlockConnection* >, m_connections );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

MaterialLayout::MaterialLayout( const FilePath& resourceName )
   : TGraphLayout< Material, MaterialNode >( resourceName )
{
}

///////////////////////////////////////////////////////////////////////////////

void MaterialLayout::initBlocksFactory( BlocksFactory& factory )
{
   factory.associate< MNPixelShader, MBPixelShader >();
   factory.associate< MNSpatialEntity, MBSpatialEntity >();
   factory.associate< MNCamera, MBCamera >();
   factory.associate< MNTexture, MBTexture >();
   factory.associate< MNFloat, MBFloat >();
   factory.associate< MNVec4, MBVec4 >();
   factory.associate< MNBool, MBBool >();
   factory.associate< MNInstanceTexture, MBInstanceTexture >();
   factory.associate< MNSurfaceProperties, MBSurfaceProperties >();
   factory.associate< MNTimer, MBTimer >();
   factory.associate< MNMaterialIndex, MBMaterialIndex >();
}

///////////////////////////////////////////////////////////////////////////////

void MaterialLayout::initSocketsFactory( SocketsFactory& factory )
{
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
   factory.associate< MSIntInput, MSSInt >();
   factory.associate< MSIntOutput, MSSInt >();

}

///////////////////////////////////////////////////////////////////////////////

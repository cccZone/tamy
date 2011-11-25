/// @file   core-Renderer/MaterialSockets.h
/// @brief  material graph sockets
#pragma once

#include "core/GraphBuilderSockets.h"
#include "core/Object.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialNode;
class ShaderTexture;
typedef GBNodeOutput< MaterialNode > MSOutput;

///////////////////////////////////////////////////////////////////////////////

class MSFloatInput : public TGBNodeInput< MaterialNode, float >
{
   DECLARE_CLASS( MSFloatInput )

public:
   /**
    * Constructor.
    */
   MSFloatInput( const std::string& name = "" ) : TGBNodeInput< MaterialNode, float >( name ) {}
};

class MSFloatOutput : public TGBNodeOutput< MaterialNode, float >
{
   DECLARE_CLASS( MSFloatOutput )

public:
   /**
    * Constructor.
    */
   MSFloatOutput( const std::string& name = "" ) : TGBNodeOutput< MaterialNode, float >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class MSMatrixInput : public TGBNodeInput< MaterialNode, D3DXMATRIX >
{
   DECLARE_CLASS( MSMatrixInput )

public:
   /**
    * Constructor.
    */
   MSMatrixInput( const std::string& name = "" ) : TGBNodeInput< MaterialNode, D3DXMATRIX >( name ) {}
};

class MSMatrixOutput : public TGBNodeOutput< MaterialNode, D3DXMATRIX >
{
   DECLARE_CLASS( MSMatrixOutput )

public:
   /**
    * Constructor.
    */
   MSMatrixOutput( const std::string& name = "" ) : TGBNodeOutput< MaterialNode, D3DXMATRIX >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class MSTextureInput : public TGBNodePtrInput< MaterialNode, ShaderTexture >
{
   DECLARE_CLASS( MSTextureInput )

public:
   /**
    * Constructor.
    */
   MSTextureInput( const std::string& name = "" ) : TGBNodePtrInput< MaterialNode, ShaderTexture >( name ) {}
};

class MSTextureOutput : public TGBNodePtrOutput< MaterialNode, ShaderTexture >
{
   DECLARE_CLASS( MSTextureOutput )

public:
   /**
    * Constructor.
    */
   MSTextureOutput( const std::string& name = "" ) : TGBNodePtrOutput< MaterialNode, ShaderTexture >( name ) {}
};


///////////////////////////////////////////////////////////////////////////////

class MSVec4Input : public TGBNodeInput< MaterialNode, D3DXVECTOR4 >
{
   DECLARE_CLASS( MSVec4Input )

public:
   /**
    * Constructor.
    */
   MSVec4Input( const std::string& name = "" ) : TGBNodeInput< MaterialNode, D3DXVECTOR4 >( name ) {}
};

class MSVec4Output : public TGBNodeOutput< MaterialNode, D3DXVECTOR4 >
{
   DECLARE_CLASS( MSVec4Output )

public:
   /**
    * Constructor.
    */
   MSVec4Output( const std::string& name = "" ) : TGBNodeOutput< MaterialNode, D3DXVECTOR4 >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

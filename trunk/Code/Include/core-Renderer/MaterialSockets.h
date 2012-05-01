/// @file   core-Renderer/MaterialSockets.h
/// @brief  material graph sockets
#pragma once

#include "core/GraphBuilderSockets.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class MaterialNode;
class ShaderTexture;
typedef GBNodeOutput< MaterialNode > MSOutput;

///////////////////////////////////////////////////////////////////////////////

class MSFloatInput : public TGBNodeInput< MaterialNode, float >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSFloatInput( const std::string& name = "" ) : TGBNodeInput< MaterialNode, float >( name ) {}
};

class MSFloatOutput : public TGBNodeOutput< MaterialNode, float >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSFloatOutput( const std::string& name = "" ) : TGBNodeOutput< MaterialNode, float >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class MSMatrixInput : public TGBNodeInput< MaterialNode, D3DXMATRIX >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSMatrixInput( const std::string& name = "" ) : TGBNodeInput< MaterialNode, D3DXMATRIX >( name ) {}
};

class MSMatrixOutput : public TGBNodeOutput< MaterialNode, D3DXMATRIX >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSMatrixOutput( const std::string& name = "" ) : TGBNodeOutput< MaterialNode, D3DXMATRIX >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class MSTextureInput : public TGBNodePtrInput< MaterialNode, ShaderTexture >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSTextureInput( const std::string& name = "" ) : TGBNodePtrInput< MaterialNode, ShaderTexture >( name ) {}
};

class MSTextureOutput : public TGBNodePtrOutput< MaterialNode, ShaderTexture >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSTextureOutput( const std::string& name = "" ) : TGBNodePtrOutput< MaterialNode, ShaderTexture >( name ) {}
};


///////////////////////////////////////////////////////////////////////////////

class MSVec4Input : public TGBNodeInput< MaterialNode, D3DXVECTOR4 >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSVec4Input( const std::string& name = "" ) : TGBNodeInput< MaterialNode, D3DXVECTOR4 >( name ) {}
};

class MSVec4Output : public TGBNodeOutput< MaterialNode, D3DXVECTOR4 >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSVec4Output( const std::string& name = "" ) : TGBNodeOutput< MaterialNode, D3DXVECTOR4 >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class MSBoolInput : public TGBNodeInput< MaterialNode, bool >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSBoolInput( const std::string& name = "" ) : TGBNodeInput< MaterialNode, bool >( name ) {}
};

class MSBoolOutput : public TGBNodeOutput< MaterialNode, bool >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSBoolOutput( const std::string& name = "" ) : TGBNodeOutput< MaterialNode, bool >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

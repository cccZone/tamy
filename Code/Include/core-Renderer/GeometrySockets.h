/// @file   core-Renderer/GeometrylSockets.h
/// @brief  geometry shader graph sockets
#pragma once

#include "core/GraphBuilderSockets.h"
#include "core/Object.h"


///////////////////////////////////////////////////////////////////////////////

class GeometryShaderNode;
class ShaderTexture;
typedef GBNodeOutput< GeometryShaderNode > GSOutput;

///////////////////////////////////////////////////////////////////////////////

class GSFloatInput : public TGBNodeInput< GeometryShaderNode, float >
{
   DECLARE_CLASS( GSFloatInput )

public:
   /**
    * Constructor.
    */
   GSFloatInput( const std::string& name = "" ) : TGBNodeInput< GeometryShaderNode, float >( name ) {}
};

class GSFloatOutput : public TGBNodeOutput< GeometryShaderNode, float >
{
   DECLARE_CLASS( GSFloatOutput )

public:
   /**
    * Constructor.
    */
   GSFloatOutput( const std::string& name = "" ) : TGBNodeOutput< GeometryShaderNode, float >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class GSMatrixInput : public TGBNodeInput< GeometryShaderNode, D3DXMATRIX >
{
   DECLARE_CLASS( GSMatrixInput )

public:
   /**
    * Constructor.
    */
   GSMatrixInput( const std::string& name = "" ) : TGBNodeInput< GeometryShaderNode, D3DXMATRIX >( name ) {}
};

class GSMatrixOutput : public TGBNodeOutput< GeometryShaderNode, D3DXMATRIX >
{
   DECLARE_CLASS( GSMatrixOutput )

public:
   /**
    * Constructor.
    */
   GSMatrixOutput( const std::string& name = "" ) : TGBNodeOutput< GeometryShaderNode, D3DXMATRIX >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class GSTextureInput : public TGBNodePtrInput< GeometryShaderNode, ShaderTexture >
{
   DECLARE_CLASS( GSTextureInput )

public:
   /**
    * Constructor.
    */
   GSTextureInput( const std::string& name = "" ) : TGBNodePtrInput< GeometryShaderNode, ShaderTexture >( name ) {}
};

class GSTextureOutput : public TGBNodePtrOutput< GeometryShaderNode, ShaderTexture >
{
   DECLARE_CLASS( GSTextureOutput )

public:
   /**
    * Constructor.
    */
   GSTextureOutput( const std::string& name = "" ) : TGBNodePtrOutput< GeometryShaderNode, ShaderTexture >( name ) {}
};


///////////////////////////////////////////////////////////////////////////////

class GSVec4Input : public TGBNodeInput< GeometryShaderNode, D3DXVECTOR4 >
{
   DECLARE_CLASS( GSVec4Input )

public:
   /**
    * Constructor.
    */
   GSVec4Input( const std::string& name = "" ) : TGBNodeInput< GeometryShaderNode, D3DXVECTOR4 >( name ) {}
};

class GSVec4Output : public TGBNodeOutput< GeometryShaderNode, D3DXVECTOR4 >
{
   DECLARE_CLASS( GSVec4Output )

public:
   /**
    * Constructor.
    */
   GSVec4Output( const std::string& name = "" ) : TGBNodeOutput< GeometryShaderNode, D3DXVECTOR4 >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class GSBoolInput : public TGBNodeInput< GeometryShaderNode, bool >
{
   DECLARE_CLASS( GSBoolInput )

public:
   /**
    * Constructor.
    */
   GSBoolInput( const std::string& name = "" ) : TGBNodeInput< GeometryShaderNode, bool >( name ) {}
};

class GSBoolOutput : public TGBNodeOutput< GeometryShaderNode, bool >
{
   DECLARE_CLASS( GSBoolOutput )

public:
   /**
    * Constructor.
    */
   GSBoolOutput( const std::string& name = "" ) : TGBNodeOutput< GeometryShaderNode, bool >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

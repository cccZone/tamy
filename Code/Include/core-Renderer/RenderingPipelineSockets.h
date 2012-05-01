/// @file   core-Renderer/RenderingPipelineSockets.h
/// @brief  rendering pipeline node base sockets definitions.
#pragma once

#include "core/GraphBuilderSockets.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineNode;
class ShaderTexture;
class RenderTarget;
typedef GBNodeOutput< RenderingPipelineNode > RPOutput;

///////////////////////////////////////////////////////////////////////////////

class RPBoolInput : public TGBNodeInput< RenderingPipelineNode, bool >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPBoolInput( const std::string& name = "" ) : TGBNodeInput< RenderingPipelineNode, bool >( name ) {}
};

class RPBoolOutput : public TGBNodeOutput< RenderingPipelineNode, bool >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPBoolOutput( const std::string& name = "" ) : TGBNodeOutput< RenderingPipelineNode, bool >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class RPFloatInput : public TGBNodeInput< RenderingPipelineNode, float >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPFloatInput( const std::string& name = "" ) : TGBNodeInput< RenderingPipelineNode, float >( name ) {}
};

class RPFloatOutput : public TGBNodeOutput< RenderingPipelineNode, float >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPFloatOutput( const std::string& name = "" ) : TGBNodeOutput< RenderingPipelineNode, float >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class RPIntInput : public TGBNodeInput< RenderingPipelineNode, int >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPIntInput( const std::string& name = "" ) : TGBNodeInput< RenderingPipelineNode, int >( name ) {}
};

class RPIntOutput : public TGBNodeOutput< RenderingPipelineNode, int >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPIntOutput( const std::string& name = "" ) : TGBNodeOutput< RenderingPipelineNode, int >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class RPMatrixInput : public TGBNodeInput< RenderingPipelineNode, D3DXMATRIX >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPMatrixInput( const std::string& name = "" ) : TGBNodeInput< RenderingPipelineNode, D3DXMATRIX >( name ) {}
};

class RPMatrixOutput : public TGBNodeOutput< RenderingPipelineNode, D3DXMATRIX >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPMatrixOutput( const std::string& name = "" ) : TGBNodeOutput< RenderingPipelineNode, D3DXMATRIX >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class RPStringInput : public TGBNodeInput< RenderingPipelineNode, std::string >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPStringInput( const std::string& name = "" ) : TGBNodeInput< RenderingPipelineNode, std::string >( name ) {}
};

class RPStringOutput : public TGBNodeOutput< RenderingPipelineNode, std::string >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPStringOutput( const std::string& name = "" ) : TGBNodeOutput< RenderingPipelineNode, std::string >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class RPTextureInput : public TGBNodePtrInput< RenderingPipelineNode, ShaderTexture >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPTextureInput( const std::string& name = "" ) : TGBNodePtrInput< RenderingPipelineNode, ShaderTexture >( name ) {}
};

class RPTextureOutput : public TGBNodePtrOutput< RenderingPipelineNode, ShaderTexture >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPTextureOutput( const std::string& name = "" ) : TGBNodePtrOutput< RenderingPipelineNode, ShaderTexture >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class RPVec4Input : public TGBNodeInput< RenderingPipelineNode, D3DXVECTOR4 >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPVec4Input( const std::string& name = "" ) : TGBNodeInput< RenderingPipelineNode, D3DXVECTOR4 >( name ) {}
};

class RPVec4Output : public TGBNodeOutput< RenderingPipelineNode, D3DXVECTOR4 >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPVec4Output( const std::string& name = "" ) : TGBNodeOutput< RenderingPipelineNode, D3DXVECTOR4 >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class RPVoidInput : public GBNodeInput< RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPVoidInput( const std::string& name = "" ) : GBNodeInput< RenderingPipelineNode >( name ) {}
};

class RPVoidOutput : public GBNodeOutput< RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   RPVoidOutput( const std::string& name = "" ) : GBNodeOutput< RenderingPipelineNode >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

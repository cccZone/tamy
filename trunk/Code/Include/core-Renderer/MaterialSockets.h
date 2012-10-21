/// @file   core-Renderer/MaterialSockets.h
/// @brief  material graph sockets
#pragma once

#include "core/GraphBuilderSockets.h"
#include "core/Matrix.h"
#include "core/Vector.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialNode;
class ShaderTexture;
typedef GBNodeOutput< MaterialNode > MSOutput;

///////////////////////////////////////////////////////////////////////////////

class MSFloatInput : public TGBNodeInput< MaterialNode, float >
{
   DECLARE_ALLOCATOR( MSFloatInput, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSFloatInput( const std::string& name = "" ) : TGBNodeInput< MaterialNode, float >( name ) {}
};

class MSFloatOutput : public TGBNodeOutput< MaterialNode, float >
{
   DECLARE_ALLOCATOR( MSFloatOutput, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSFloatOutput( const std::string& name = "" ) : TGBNodeOutput< MaterialNode, float >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class MSMatrixInput : public TGBNodeInput< MaterialNode, Matrix >
{
   DECLARE_ALLOCATOR( MSMatrixInput, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSMatrixInput( const std::string& name = "" ) : TGBNodeInput< MaterialNode, Matrix >( name ) {}
};

class MSMatrixOutput : public TGBNodeOutput< MaterialNode, Matrix >
{
   DECLARE_ALLOCATOR( MSMatrixOutput, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSMatrixOutput( const std::string& name = "" ) : TGBNodeOutput< MaterialNode, Matrix >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class MSTextureInput : public TGBNodePtrInput< MaterialNode, ShaderTexture >
{
   DECLARE_ALLOCATOR( MSTextureInput, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSTextureInput( const std::string& name = "" ) : TGBNodePtrInput< MaterialNode, ShaderTexture >( name ) {}
};

class MSTextureOutput : public TGBNodePtrOutput< MaterialNode, ShaderTexture >
{
   DECLARE_ALLOCATOR( MSTextureOutput, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSTextureOutput( const std::string& name = "" ) : TGBNodePtrOutput< MaterialNode, ShaderTexture >( name ) {}
};


///////////////////////////////////////////////////////////////////////////////

class MSVec4Input : public TGBNodeInput< MaterialNode, Vector >
{
   DECLARE_ALLOCATOR( MSVec4Input, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSVec4Input( const std::string& name = "" ) : TGBNodeInput< MaterialNode, Vector >( name ) {}
};

class MSVec4Output : public TGBNodeOutput< MaterialNode, Vector >
{
   DECLARE_ALLOCATOR( MSVec4Output, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSVec4Output( const std::string& name = "" ) : TGBNodeOutput< MaterialNode, Vector >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class MSBoolInput : public TGBNodeInput< MaterialNode, bool >
{
   DECLARE_ALLOCATOR( MSBoolInput, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSBoolInput( const std::string& name = "" ) : TGBNodeInput< MaterialNode, bool >( name ) {}
};

class MSBoolOutput : public TGBNodeOutput< MaterialNode, bool >
{
   DECLARE_ALLOCATOR( MSBoolOutput, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSBoolOutput( const std::string& name = "" ) : TGBNodeOutput< MaterialNode, bool >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class MSIntInput : public TGBNodeInput< MaterialNode, int >
{
   DECLARE_ALLOCATOR( MSIntInput, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSIntInput( const std::string& name = "" ) : TGBNodeInput< MaterialNode, int >( name ) {}
};

class MSIntOutput : public TGBNodeOutput< MaterialNode, int >
{
   DECLARE_ALLOCATOR( MSIntOutput, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   MSIntOutput( const std::string& name = "" ) : TGBNodeOutput< MaterialNode, int >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

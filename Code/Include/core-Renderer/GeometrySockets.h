/// @file   core-Renderer/GeometrylSockets.h
/// @brief  geometry shader graph sockets
#pragma once

#include "core/GraphBuilderSockets.h"
#include "core/ReflectionObject.h"
#include "core/Matrix.h"
#include "core/Vector.h"


///////////////////////////////////////////////////////////////////////////////

class GeometryShaderNode;
class ShaderTexture;
typedef GBNodeOutput< GeometryShaderNode > GSOutput;

///////////////////////////////////////////////////////////////////////////////

class GSFloatInput : public TGBNodeInput< GeometryShaderNode, float >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   GSFloatInput( const std::string& name = "" ) : TGBNodeInput< GeometryShaderNode, float >( name ) {}
};

class GSFloatOutput : public TGBNodeOutput< GeometryShaderNode, float >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   GSFloatOutput( const std::string& name = "" ) : TGBNodeOutput< GeometryShaderNode, float >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class GSMatrixInput : public TGBNodeInput< GeometryShaderNode, Matrix >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   GSMatrixInput( const std::string& name = "" ) : TGBNodeInput< GeometryShaderNode, Matrix >( name ) {}
};

class GSMatrixOutput : public TGBNodeOutput< GeometryShaderNode, Matrix >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   GSMatrixOutput( const std::string& name = "" ) : TGBNodeOutput< GeometryShaderNode, Matrix >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class GSTextureInput : public TGBNodePtrInput< GeometryShaderNode, ShaderTexture >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   GSTextureInput( const std::string& name = "" ) : TGBNodePtrInput< GeometryShaderNode, ShaderTexture >( name ) {}
};

class GSTextureOutput : public TGBNodePtrOutput< GeometryShaderNode, ShaderTexture >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   GSTextureOutput( const std::string& name = "" ) : TGBNodePtrOutput< GeometryShaderNode, ShaderTexture >( name ) {}
};


///////////////////////////////////////////////////////////////////////////////

class GSVec4Input : public TGBNodeInput< GeometryShaderNode, Vector >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   GSVec4Input( const std::string& name = "" ) : TGBNodeInput< GeometryShaderNode, Vector >( name ) {}
};

class GSVec4Output : public TGBNodeOutput< GeometryShaderNode, Vector >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   GSVec4Output( const std::string& name = "" ) : TGBNodeOutput< GeometryShaderNode, Vector >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

class GSBoolInput : public TGBNodeInput< GeometryShaderNode, bool >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   GSBoolInput( const std::string& name = "" ) : TGBNodeInput< GeometryShaderNode, bool >( name ) {}
};

class GSBoolOutput : public TGBNodeOutput< GeometryShaderNode, bool >
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    */
   GSBoolOutput( const std::string& name = "" ) : TGBNodeOutput< GeometryShaderNode, bool >( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

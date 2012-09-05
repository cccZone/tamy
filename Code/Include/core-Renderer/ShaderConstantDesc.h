/// @file   core-Renderer/ShaderConstantDesc.h
/// @brief  description of a shader constant
#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

class InStream;
class OutStream;

///////////////////////////////////////////////////////////////////////////////

/**
 * This structure describes a single shader constant.
 */
struct ShaderConstantDesc
{
   enum Type
   {
      CT_Bool,
      CT_Int,
      CT_Float,
      CT_String,
      CT_Matrix,
      CT_Vec4,
      CT_Texture
   };

   Type              m_type;
   std::string       m_name;

   // -------------------------------------------------------------------------
   // Operators
   // -------------------------------------------------------------------------
   bool operator==( const ShaderConstantDesc& rhs ) const;

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend OutStream& operator<<( OutStream& stream, const ShaderConstantDesc& params );
   friend InStream& operator>>( InStream& stream, ShaderConstantDesc& params );
};

///////////////////////////////////////////////////////////////////////////////

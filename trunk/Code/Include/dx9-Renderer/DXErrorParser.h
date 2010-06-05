#pragma once

/// @file   dx9-Renderer\DXErrorParser.h
/// @brief  a utility for creating meaningful error messages based on 
///         DX error codes

#include <string>
#include <d3dx9.h>

///////////////////////////////////////////////////////////////////////////////

std::string translateDxError( const std::string& desc, HRESULT errCode );

///////////////////////////////////////////////////////////////////////////////


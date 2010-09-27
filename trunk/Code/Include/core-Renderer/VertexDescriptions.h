/// @file      core-Renderer\VertexDescriptions.h
/// @brief     a list of all vertex descriptions
#pragma once

#include <d3dx9.h>


enum VertexDescId
{
   VDI_SIMPLE,
   VDI_SIMPLE_SKINNING,
   VDI_MAX,
};

extern D3DVERTEXELEMENT9* g_vertexDescriptions[];

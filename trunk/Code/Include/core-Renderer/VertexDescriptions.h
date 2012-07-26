/// @file      core-Renderer\VertexDescriptions.h
/// @brief     a list of all vertex descriptions
#pragma once

#include "core/ReflectionEnum.h"
#include "core-Renderer/VertexDescriptors.h"

///////////////////////////////////////////////////////////////////////////////

enum VertexDescId
{
   VDI_SIMPLE,
   VDI_SIMPLE_SKINNING,
   VDI_SIMPLE_SCREENSPACE,
   VDI_DEBUG_GEOMETRY,
   VDI_LIGHT,
   VDI_MAX,
};

///////////////////////////////////////////////////////////////////////////////

extern VertexDescriptor* g_vertexDescriptions[];

///////////////////////////////////////////////////////////////////////////////

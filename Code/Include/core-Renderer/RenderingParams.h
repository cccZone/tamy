/// @file   core-Renderer/RenderingParams.h
/// @brief  texture parameters definitions
#pragma once


///////////////////////////////////////////////////////////////////////////////

enum TextureAddressingMode
{
   TAM_WRAP            = 1,
   TAM_MIRROR          = 2,
   TAM_CLAMP           = 3,
   TAM_BORDER          = 4,
   TAM_MIRRORONCE      = 5,
   TAM_FORCE_BYTE      = 0xff
};

///////////////////////////////////////////////////////////////////////////////

enum TextureFilteringMode
{
   TFM_NONE                = 0,    // filtering disabled
   TFM_POINT               = 1,    // nearest
   TFM_LINEAR              = 2,    // linear interpolation
   TFM_ANISOTROPIC         = 3,    // anisotropic
   TFM_FORCE_BYTE          = 0xff
};

///////////////////////////////////////////////////////////////////////////////

enum CullingMode 
{
   CULL_NONE                = 1,
   CULL_CW                  = 2,
   CULL_CCW                 = 3,
   CULL_FORCE_BYTE          = 0xff, /* force 32-bit size enum */
};

///////////////////////////////////////////////////////////////////////////////

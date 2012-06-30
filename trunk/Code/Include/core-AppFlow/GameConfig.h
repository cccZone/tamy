/// @file   core-AppFlow/GameConfig.h
/// @brief  a game config resource
#pragma once

#include "core/Resource.h"
#include "core/FilePath.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A game config resource.
 */
struct GameConfig : public Resource
{
   DECLARE_RESOURCE();

public:

   FilePath       m_renderingPipelinePath;
   FilePath       m_worldModelPath;
};

///////////////////////////////////////////////////////////////////////////////

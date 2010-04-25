#pragma once

/// @file   core-Renderer\EffectAttribute.h
/// @brief  attribute an effect features


///////////////////////////////////////////////////////////////////////////////

/**
 * An effect can feature many attributes which tell the renderer
 * what the effect does and how it should be managed / sorted
 * in the render queue.
 */
class EffectAttribute
{
public:
   virtual ~EffectAttribute() {}
};

///////////////////////////////////////////////////////////////////////////////

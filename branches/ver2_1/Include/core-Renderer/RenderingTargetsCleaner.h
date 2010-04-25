#pragma once

/// @file   core-Renderer\RenderingTargetsCleaner.h
/// @brief  a class capable of cleaning the contents of currently set
///         rendering targets


///////////////////////////////////////////////////////////////////////////////

/**
 * An implementation of this interface cleans the currently used
 * rendering targets.
 */
class RenderingTargetsCleaner
{
public:
   virtual ~RenderingTargetsCleaner() {}

   /**
    * The method cleans the specified number of rendering targets.
    *
    * @param targetsCount  number of rendering targets we want cleaned
    */
   virtual void cleanRenderingTargets(unsigned int targetsCount) = 0;
};

///////////////////////////////////////////////////////////////////////////////

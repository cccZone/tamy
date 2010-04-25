#pragma once

/// @file   core-Renderer\TargetsCleaningMechanism.h
/// @brief  a mechanism for cleaning rendering targets in the rendering
///         pipeline

#include "core-Renderer\RenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingTargetsCleaner;
class RenderingTargetsPolicy;

///////////////////////////////////////////////////////////////////////////////

/**
 * This mechanism cleans all the rendering targets
 * the policy it's set up with has.
 */
class TargetsCleaningMechanism : public RenderingMechanism
{
private:
   RenderingTargetsPolicy* m_policy;
   RenderingTargetsCleaner* m_cleaner;

public:
   TargetsCleaningMechanism(RenderingTargetsPolicy* policy, 
                            RenderingTargetsCleaner* cleaner);
   ~TargetsCleaningMechanism();

   void render();
};

///////////////////////////////////////////////////////////////////////////////

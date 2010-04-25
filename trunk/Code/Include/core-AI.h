#pragma once

/// @file   core-AI.h
/// @brief  aggregate include file for the 'core-AI' project


// ----------------------------------------------------------------------------
// ControlStructures
// ----------------------------------------------------------------------------
#include "core-AI\BTNode.h"
#include "core-AI\BTCompositeNode.h"
#include "core-AI\BTSelectorNode.h"
#include "core-AI\BTSequenceNode.h"
#include "core-AI\BTNodeSelectionStrategy.h"
#include "core-AI\BTPrecondition.h"
#include "core-AI\PreconditionSelectionStrategy.h"
#include "core-AI\FsmLogicState.h"
#include "core-AI\FsmLogicStateController.h"

// ----------------------------------------------------------------------------
// MessagingSystem
// ----------------------------------------------------------------------------
#include "core-AI\Messagable.h"
#include "core-AI\Message.h"
#include "core-AI\MessageDispatcher.h"

// ----------------------------------------------------------------------------
// SteeringBehaviors
// ----------------------------------------------------------------------------
#include "core-AI\SteeringBehavior.h"
#include "core-AI\CompositeSteeringBehavior.h"

#pragma once

/// @file   core-AppFlow.h
/// @brief  aggregate include file for the 'core-AppFlow' project


// ----------------------------------------------------------------------------
// Blackboard
// ----------------------------------------------------------------------------
#include "core-AppFlow\ApplicationData.h"

// ----------------------------------------------------------------------------
// Core
// ----------------------------------------------------------------------------
#include "core-AppFlow\Application.h"
#include "core-AppFlow\ApplicationManager.h"
#include "core-AppFlow\ExecutionContext.h"

// ----------------------------------------------------------------------------
// TimeController
// ----------------------------------------------------------------------------
#include "core-AppFlow\TimeController.h"
#include "core-AppFlow\TimeControllerTrack.h"
#include "core-AppFlow\TimeDependent.h"
#include "core-AppFlow\TimeEvent.h"

// ----------------------------------------------------------------------------
// UserInputController
// ----------------------------------------------------------------------------
#include "core-AppFlow\UserInputController.h"
#include "core-AppFlow\KeysStatusModel.h"
#include "core-AppFlow\KeyStatusHandler.h"
#include "core-AppFlow\KeysStatusManager.h"

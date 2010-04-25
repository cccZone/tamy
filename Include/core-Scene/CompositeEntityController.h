#pragma once

/// @file   core-Scene\CompositeEntityController.h
/// @brief  a composite for aggregating individual entity controllers

#include <vector>
#include "core-Scene\EntityController.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This controller implementations allows to execute multiple controllers
 * using the same time base.
 */
class CompositeEntityController : public EntityController
{
private:
   typedef std::vector<EntityController*> Controllers;
   Controllers m_controllers;

public:
   ~CompositeEntityController();

   /**
    * Adds a new controller to the composite
    *
    * @param controller    a controller we want to execute along with others
    */
   void add(EntityController* controller);

   // -------------------------------------------------------------------------
   // EntityController implementation
   // -------------------------------------------------------------------------
   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

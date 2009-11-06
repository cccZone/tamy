#pragma once

/// @file   core-Scene\ModelSerializer.h
/// @brief  tool for saving/loading model contents to external storage

#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This class has the ability to save the state of the model (all of its
 * entities etc.) and restore it from an external device.
 */
class ModelSerializer
{
public:
   virtual ~ModelSerializer() {}

   /**
    * The method saves an integer.
    *
    * @param val     value to save
    */
   virtual void saveInt(int val) = 0;

   /**
    * Restores the next integer from the underlying stream of data.
    *
    * @return        serialized integer value
    */
   virtual int loadInt() = 0;
};

///////////////////////////////////////////////////////////////////////////////

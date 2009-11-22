#pragma once

/// @file   core-Scene\Situated.h
/// @brief  marker interface for all entities with situation

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Situated
{
public:
   virtual ~Situated() {}

   /**
    * Should return entity's current world situation matrix.
    *
    * @return     matrix with entity's world situation
    */
   virtual const D3DXMATRIX& getSituation() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

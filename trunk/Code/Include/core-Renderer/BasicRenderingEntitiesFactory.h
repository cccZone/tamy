/// @file   core-Renderer/BasicRenderingEntitiesFactory.h
/// @brief  factory class that creates common rendering entities, such as materials, geometry etc. and sets them up i
#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

class MaterialEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * A factory class that creates common rendering entities, such as materials, geometry etc. and sets them up with
 * common engine resources.
 */
class BasicRenderingEntitiesFactory
{
public:
   /**
    * Creates a basic material entity.
    *
    * @param name
    */
   static MaterialEntity* createMaterial( const std::string& name = "" );
};

///////////////////////////////////////////////////////////////////////////////

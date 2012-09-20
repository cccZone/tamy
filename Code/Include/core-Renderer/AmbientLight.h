/// @file   core-Renderer/AmbientLight.h
/// @brief  an entity that provides a uniform, ambient lighting for the scene
#pragma once

#include "core-Renderer\Light.h"
#include "core\Color.h"
#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * An entity that provides a uniform, ambient lighting for the scene.
 */
class AmbientLight : public SpatialEntity, public UniqueObject< AmbientLight >
{
   DECLARE_CLASS();

public:
   Color             m_lightColor;

public:
   /**
    * Constructor.
    */
   AmbientLight( const std::string& name = "" );
};

///////////////////////////////////////////////////////////////////////////////

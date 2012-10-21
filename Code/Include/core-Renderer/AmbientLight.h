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
   DECLARE_ALLOCATOR( AmbientLight, AM_ALIGNED_16 );
   DECLARE_CLASS();

public:
   Color             m_lightColor;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   AmbientLight( const std::string& name = "" );

   /**
    * Copy constructor.
    *
    * @param rhs
    */
   AmbientLight( const AmbientLight& rhs );

protected:
   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   Entity* cloneSelf() const;
};

///////////////////////////////////////////////////////////////////////////////

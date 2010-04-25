#pragma once

/// @file   core-Renderer\RendererObjectImpl.h
/// @brief  base interface for all renderer objects implementations


/////////////////////////////////////////////////////////////////////////////////////////

class Renderer;

/////////////////////////////////////////////////////////////////////////////////////////

/**
 * All renderer objects implementations should implement this interface.
 * It allows to properly initialize such a representation when it's created.
 */
class RendererObjectImpl
{
public:
   virtual ~RendererObjectImpl() {}

   /**
    * Initializes an implementation using a renderer.
    * A specialized instance of a renderer will be passed here, so particular 
    * implementations can safely cast to the Renderer implementations they
    * are designed to work with.
    *
    * @param renderer   renderer that created this implementation
    */
   virtual void initialize(Renderer& renderer) {}
};

/////////////////////////////////////////////////////////////////////////////////////////

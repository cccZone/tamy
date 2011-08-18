#pragma once

/// @file   TamyEngine\QueryableEntity.h
/// @brief  An entity representation which can be picked of the screen 
///         by pointing at it


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * An entity representation which can be picked of the screen by pointing at it.
 */
class QueryableEntity
{
public:
   virtual ~QueryableEntity() {}

   /**
    * Renders the geometry.
    *
    * @param renderer
    * @param shader
    */
   virtual void render( Renderer& renderer, PixelShader& shader ) = 0;

};

///////////////////////////////////////////////////////////////////////////////

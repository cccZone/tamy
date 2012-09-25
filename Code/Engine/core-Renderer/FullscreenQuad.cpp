#include "core-Renderer\FullscreenQuad.h"


///////////////////////////////////////////////////////////////////////////////

RCFullscreenQuad::RCFullscreenQuad( uint width, uint height ) 
   : m_width( width )
   , m_height( height ) 
{}

///////////////////////////////////////////////////////////////////////////////

RCScreenQuad::RCScreenQuad( uint offsetX, uint offsetY, uint width, uint height )
   : m_offsetX( offsetX )
   , m_offsetY( offsetY )
   , m_width( width )
   , m_height( height )
{
}

///////////////////////////////////////////////////////////////////////////////

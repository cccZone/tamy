#pragma once

#include "core-Renderer\Overlay.h"


namespace demo
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This wrapper will make sure we can have the same overlay rendered
 * by multiple mechanisms, and still have it managed from outside.
 */
class SharedOverlay : public Overlay
{
private:
   Overlay& m_overlay;

public:
   SharedOverlay(Overlay& overlay) : m_overlay(overlay) {}

   void render() {m_overlay.render();}
};

///////////////////////////////////////////////////////////////////////////////

} // namespace demo

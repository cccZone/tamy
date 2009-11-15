#pragma once

/// @file   ext-RendererView\SkyBox.h
/// @brief  graphical representation of a sky box scenery.

#include "ext-RendererView\GraphicalRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

struct SkyBoxEntity;

///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a graphical representation of a sky box scenery.
 */
class SkyBox : public GraphicalRepresentation
{
private:
   SkyBoxEntity& m_entity;

public:
   /**
    * Constructor.
    * 
    * @param entity     sky box entity this representation is for
    */
   SkyBox(SkyBoxEntity& entity);
   ~SkyBox();

   // -------------------------------------------------------------------------
   // GraphicalRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize(RendererView& rendererView);

   void deinitialize(RendererView& rendererView);

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer

#pragma once

/// @file   core-Renderer\RenderMaterial.h
/// @brief  rendering action that sets materials from the rendered renderable
///         during rendering process

#include "core-Renderer\RenderingAction.h"


///////////////////////////////////////////////////////////////////////////////

class Material;

///////////////////////////////////////////////////////////////////////////////

/**
 * This action sets up a material for rendering, batching
 * the materials if possible.
 *
 * The class memorizes the last material used.
 * When a material is set, it means that there was a memory
 * transfer involved that we don't want to repeat.
 * That's why the class memorizes the last material used
 * and sets up a material only if it's different from the memorized
 * one.
 */
class RenderMaterial : public RenderingAction
{
private:
   // This class level variable stores an instance of the last material set.
   // Since a rendering pipeline can have many instances of this class set,
   // if this was to be an instance member, the material that was set memory would 
   // be corrupt - i.e. one instance would be dealing only with object
   // with matA, and another only with ones with matB. Now if we were
   // to render that, each would set the material once, memorize it, and then
   // given that the materials they deal with don't change - no changes would
   // be present.
   // However - outside the mechanisms change, one rendering its part, the other
   // one rendering its - so the higher level policy reuqires the materials to change.
   // Thus the need to make this a global level variable.
   //
   // TODO: should more classes be affected by the global rendering mechanisms
   // changing policy, create a mechanism that deals with it and extract this
   // memory-related functionality from this class there.
   static const Material* m_prevMaterial;

public:
   RenderMaterial();

   /**
    * Resets the memory of the last material used.
    * (For testing purposes mainly)
    */
   static void resetMaterialMemory();

   void onRenderingBegin(Renderable& renderable, RendererImpl& impl);

   void onRenderingEnd(Renderable& renderable, RendererImpl& impl);
};

///////////////////////////////////////////////////////////////////////////////

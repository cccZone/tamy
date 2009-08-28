#pragma once

#include "core-Renderer\RenderablesStorage.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class SkyBoxSide;

///////////////////////////////////////////////////////////////////////////////

/**
 * This storage is dedicated to holding a single skybox.
 * A skybox comprises 6 different renderables - one representing
 * each side of the box. Thus in order to have a skybox rendered,
 * and this means fullfilling the requirement of the rendering mechanism
 * that all renderables need to be passed in an array, we're using this
 * class, creating an instance for each skybox.
 */
class SkyBoxStorage : public RenderablesStorage
{
private:
   Array<SkyBoxSide*> m_sides;

public:
   SkyBoxStorage(SkyBoxSide* left, SkyBoxSide* right,
                 SkyBoxSide* top, SkyBoxSide* bottom,
                 SkyBoxSide* front, SkyBoxSide* back);
   ~SkyBoxStorage();

   void query(Camera& camera, Array<Renderable*>& outRenderables);
};

///////////////////////////////////////////////////////////////////////////////

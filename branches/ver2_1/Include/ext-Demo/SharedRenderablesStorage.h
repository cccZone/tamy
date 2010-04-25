#pragma once

#include "core-Renderer\RenderablesStorage.h"


namespace demo
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This wrapper will make sure we can have the same renderables storage
 * rendered by multiple mechanisms, and still have it managed from outside.
 */
class SharedRenderablesStorage : public RenderablesStorage
{
private:
   RenderablesStorage& m_storage;

public:
   SharedRenderablesStorage(RenderablesStorage& storage) : m_storage(storage) {}
   
   void query(Camera& camera, Array<Renderable*>& outNodes)
   {
      return m_storage.query(camera, outNodes);
   }
};

///////////////////////////////////////////////////////////////////////////////

} // namespace demo

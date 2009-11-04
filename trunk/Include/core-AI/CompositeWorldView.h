#pragma once

#include "core-AI\GameWorldView.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

/**
 * This composite allows to register multiple views with a game world
 * as if they were a single view.
 */
class CompositeWorldView : public GameWorldView
{
private:
   typedef std::list<GameWorldView*> ViewsList;
   ViewsList m_views;

public:
   void add(GameWorldView& view);

   void onEntityAdded(Entity& entity);

   void onEntityRemoved(Entity& entity);
};

///////////////////////////////////////////////////////////////////////////////

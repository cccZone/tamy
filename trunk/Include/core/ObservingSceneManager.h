#pragma once

#include "core\AbstractSceneManager.h"
#include "core\NodeObserver.h"


///////////////////////////////////////////////////////////////////////////////

class AbstractSceneManager;

///////////////////////////////////////////////////////////////////////////////

class ObservingSceneManager : public AbstractSceneManager, 
                              public NodeObserver
{
private:
   AbstractSceneManager* m_sceneManager;

public:
   ObservingSceneManager(AbstractSceneManager* sceneManager);
   ~ObservingSceneManager();

   void addNode(Node* node);

   void removeNode(Node& node);

   void childAdded(Node& parent, Node& child);

   void childRemoved(Node& parent, Node& child);
};

///////////////////////////////////////////////////////////////////////////////

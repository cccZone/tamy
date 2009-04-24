#include "core\ObservingSceneManager.h"
#include "core\AbstractSceneManager.h"
#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

ObservingSceneManager::ObservingSceneManager(AbstractSceneManager* sceneManager)
      : m_sceneManager(sceneManager)
{
}

///////////////////////////////////////////////////////////////////////////////

ObservingSceneManager::~ObservingSceneManager()
{
   delete m_sceneManager;
   m_sceneManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ObservingSceneManager::addNode(Node* node)
{
   node->attachObserver(*this);
   m_sceneManager->addNode(node);
}

///////////////////////////////////////////////////////////////////////////////

void ObservingSceneManager::removeNode(Node& node)
{
   node.detachObserver(*this);
   m_sceneManager->removeNode(node);
}

///////////////////////////////////////////////////////////////////////////////

void ObservingSceneManager::childAdded(Node& parent, Node& child)
{
   m_sceneManager->addNode(&child);
}

///////////////////////////////////////////////////////////////////////////////

void ObservingSceneManager::childRemoved(Node& parent, Node& child)
{
   m_sceneManager->removeNode(child);
}

///////////////////////////////////////////////////////////////////////////////
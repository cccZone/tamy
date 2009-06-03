#include "core\CompositeSceneManager.h"
#include "core\AbstractSceneManager.h"
#include "core\ObservingSceneManager.h"
#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

CompositeSceneManager::CompositeSceneManager()
      : m_rootNode(new Node("rootNode"))
{
}

///////////////////////////////////////////////////////////////////////////////

CompositeSceneManager::~CompositeSceneManager()
{
   for (std::list<AbstractSceneManager*>::iterator it = m_managers.begin();
        it != m_managers.end(); ++it)
   {
      delete *it;
   }
   m_managers.clear();

   delete m_rootNode;
   m_rootNode = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void CompositeSceneManager::addSceneManager(AbstractSceneManager* sceneManager)
{
   m_managers.push_back(new ObservingSceneManager(sceneManager));
}

///////////////////////////////////////////////////////////////////////////////

void CompositeSceneManager::addNode(Node* node)
{
   m_rootNode->addChild(node);

   for (std::list<AbstractSceneManager*>::iterator it = m_managers.begin();
        it != m_managers.end(); ++it)
   {
      (*it)->addNode(node);
   }
}

///////////////////////////////////////////////////////////////////////////////

void CompositeSceneManager::removeNode(Node& node)
{
   m_rootNode->removeChild(node);

   for (std::list<AbstractSceneManager*>::iterator it = m_managers.begin();
        it != m_managers.end(); ++it)
   {
      (*it)->removeNode(node);
   }
}

///////////////////////////////////////////////////////////////////////////////

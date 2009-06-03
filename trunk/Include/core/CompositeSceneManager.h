#pragma once

#include "core\SceneManager.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

class AbstractSceneManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents an aggregation of multiple scene aspects.
 * All the nodes will be gathered in one hierarchy, but each leaf
 * can represent a single aspect of the scene - audio representation,
 * visual representation etc.
 */
class CompositeSceneManager : public SceneManager
{
private:
   Node* m_rootNode;
   std::list<AbstractSceneManager*> m_managers;

public:
   CompositeSceneManager();
   ~CompositeSceneManager();

   void addNode(Node* node);

   void removeNode(Node& node);

   /**
    * We can't aggregate the aggregates - there's no point in doing that.
    * That's why we allow to aggregate only the leaf scene managers 
    * (namely the AbstractSceneManagers)
    */
   void addSceneManager(AbstractSceneManager* sceneManager);

   inline Node& root() {return *m_rootNode;}
};

///////////////////////////////////////////////////////////////////////////////

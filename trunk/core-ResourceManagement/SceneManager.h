#pragma once

#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Node;

///////////////////////////////////////////////////////////////////////////////

class SceneManager
{
private:
   Node* m_rootNode;

public:
   SceneManager();
   ~SceneManager();

   inline Node& getRootNode() {return *m_rootNode;}

   /**
    * This method adds a new node to the scene.
    * Depending on the implementation, different strategies
    * for doing that will be implemented
    */
   virtual void addNode(Node* node) = 0;
};

///////////////////////////////////////////////////////////////////////////////

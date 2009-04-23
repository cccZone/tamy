#pragma once


///////////////////////////////////////////////////////////////////////////////

class Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * A scene manager maintains a hierarchy of nodes. There can be many
 * different scene managers, maniaging the visual, audio, ai etc. aspects
 * of the scene in multiple ways
 */
class SceneManager
{
public:
   virtual ~SceneManager() {}

   /**
    * This method adds a new node to the scene.
    */
   virtual void addNode(Node* node) = 0;

   /**
    * This method removes an existing node from the scene
    */
   virtual void removeNode(Node& node) = 0;

   /**
    * The method is used to connect two nodes into a hierarchy
    * while making sure all the registered scene managers get updated
    */
   void connectNodes(Node& parent, Node* child);
};

///////////////////////////////////////////////////////////////////////////////

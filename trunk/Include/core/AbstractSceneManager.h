#pragma once

#include "core\SceneManager.h"
#include "core\Node.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core\NodeObserver.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a leaf scene manager - it implements the management
 * of a single aspect of a scene - audio, visual, AI etc..
 */
class AbstractSceneManager : public SceneManager,
                             public NodeVisitor
{
private:
   bool m_adding;

public:
   virtual ~AbstractSceneManager() {}

   void addNode(Node* node)
   {
      m_adding = true;
      node->accept(*this);
   }

   void removeNode(Node& node)
   {
      m_adding = false;
      node.accept(*this);
   }

   inline bool isAdding() const {return m_adding;}

protected:
   AbstractSceneManager() : m_adding(true) {}
};

///////////////////////////////////////////////////////////////////////////////

template<typename NodeType>
class SceneAspectManager : public TNodesVisitor<NodeType>
{
private:
   AbstractSceneManager* m_controller;
   
public:
   virtual ~SceneAspectManager() {}

   void visit(NodeType& node)
   {
      ASSERT(m_controller != NULL, "Class not initialized. Set the controller!!!");

      if (m_controller->isAdding())
      {
         add(node);
      }
      else
      {
         remove(node);
      }
   }

   void setController(AbstractSceneManager& controller) 
   {
      m_controller = &controller;
   }

protected:
   SceneAspectManager() : m_controller(NULL) {}

   virtual void add(NodeType& node) = 0;

   virtual void remove(NodeType& node) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
* You NEED TO invoke this macro in the scene manager class's constructor
* in order for it to work properly
*/
#define REGISTER_SCENE_ASPECT(NodeType) ((SceneAspectManager<NodeType>*)(this))->setController(*this);

///////////////////////////////////////////////////////////////////////////////
